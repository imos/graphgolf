#ifndef GRAPHGOLF_FAST_EVALUATION_GRAPH_H_
#define GRAPHGOLF_FAST_EVALUATION_GRAPH_H_
#include "base/base.h"
#include "graphgolf/graph.h"
#include "graphgolf/slice.h"

class FastEvaluationGraph : public Graph {
 public:
  typedef pair<Vertex, Vertex> Edge;

  FastEvaluationGraph(Vertex order, Degree degree)
      : Graph(order, degree),
        distance_sum_(0) {
    for (Vertex v1 = 0; v1 < order; v1++) {
      for (Vertex v2 = v1 + 1; v2 < order; v2++) {
        distance_and_routes_[0].insert(Route(v1, v2));
      }
    }
  }

  virtual ~FastEvaluationGraph() {}

  bool AddEdge(Vertex v1, Vertex v2, bool check) override;

  bool RemoveEdge(Vertex v1, Vertex v2, bool check) override;

  Score Evaluate() const override {
    if (distance_and_routes_.count(0) > 0) {
      return Score(order(), 0);
    }

    return Score(distance_and_routes_.rbegin()->first, distance_sum_ * 2);
  }

 private:
  static inline Route RegularizedRoute(const Route& route) {
    if (route.first <= route.second) return route;
    return Route(route.second, route.first);
  }

  int RouteSize(const Route& route) const {
    if (route.first == route.second) return 0;
    auto actual_route = routes_.find(RegularizedRoute(route));
    if (actual_route == routes_.end() ||
        actual_route->second.size() == 0) return -1;
    return actual_route->second.size();
  }

  bool HasRoute(const Route& route) const {
    return RouteSize(route) >= 0;
  }

  bool UpdateRoute(const Route& reference_route, const Route& additional_edge) {
    DCHECK_EQ(reference_route.second, additional_edge.first);
    DCHECK(HasRoute(reference_route))
        << "No such a route: "
        << reference_route.first << " => " << reference_route.second;
    DCHECK(HasEdge(additional_edge.first, additional_edge.second))
        << "No such an edge: "
        << additional_edge.first << " => " << additional_edge.second;

    if (reference_route.first == additional_edge.second) { return false; }
    Route additional_route =
        RegularizedRoute(Route(reference_route.first, additional_edge.second));
    set<Route> route = routes_[additional_route];
    if (route.size() != 0 && RouteSize(reference_route) + 1 >= route.size()) {
      return false;
    }

    set<Route> new_route = routes_[RegularizedRoute(reference_route)];
    DCHECK(new_route.size() != 0 ||
           reference_route.first == reference_route.second)
        << "new_route.size() = " << new_route.size() << ", "
        << "reference_route = (" <<  reference_route.first << ", "
        << reference_route.second << ")";
    if (new_route.count(RegularizedRoute(additional_edge)) != 0) {
      return false;
    }

    for (const Route& edge : route) {
      DCHECK_EQ(1, reverse_routes_[edge].count(additional_route));
      reverse_routes_[edge].erase(additional_route);
    }

    // set<Route> new_route = routes_[RegularizedRoute(reference_route)];
    if (new_route.count(RegularizedRoute(additional_edge)) != 0) {
      return false;
    }
    DCHECK_EQ(0, new_route.count(RegularizedRoute(additional_edge)));
    new_route.insert(RegularizedRoute(additional_edge));
    for (const Route& edge : new_route) {
      DCHECK_EQ(0, reverse_routes_[edge].count(additional_route));
      reverse_routes_[edge].insert(additional_route);
    }

    distance_sum_ -= route.size();
    DCHECK_EQ(1, distance_and_routes_[route.size()].count(additional_route));
    distance_and_routes_[route.size()].erase(additional_route);
    if (distance_and_routes_[route.size()].size() == 0) {
      distance_and_routes_.erase(route.size());
    }
    distance_sum_ += new_route.size();
    DCHECK_EQ(0, distance_and_routes_[new_route.size()]
                     .count(additional_route));
    distance_and_routes_[new_route.size()].insert(additional_route);
    routes_[additional_route] = std::move(new_route);

    for (Vertex vertex : Edges(additional_edge.second)) {
      UpdateRoute(Route(reference_route.first, additional_edge.second),
                  Route(additional_edge.second, vertex));
    }
    for (Vertex vertex : Edges(reference_route.first)) {
      UpdateRoute(Route(additional_edge.second, reference_route.first),
                  Route(reference_route.first, vertex));
    }

    return true;
  }

  void UpdateRoutesForEdge(const Route& edge) {
    DCHECK(HasEdge(edge.first, edge.second))
        << "No such an edge: " << edge.first << " => " << edge.second;

    UpdateRoute(Route(edge.first, edge.first), edge);
  }

  void AddRoute(const Route& reference_route, const Route& additional_edge) {
    DCHECK(!HasRoute(Route(reference_route.first, additional_edge.second)))
        << "Existing route: "
        << reference_route.first << " => " << additional_edge.second;

    bool result = UpdateRoute(reference_route, additional_edge);
    CHECK(result)
        << "Failed to add a new route: " << reference_route.first << " =("
        << reference_route.second << ")=> " << additional_edge.second;
  }

  void RemoveRoute(const Route& route) {
    DCHECK(HasRoute(route))
        << "No such a route: " << route.first << " => " << route.second;

    Route regularized_route = RegularizedRoute(route);
    for (const Edge& edge : routes_[regularized_route]) {
      DCHECK_NE(0, reverse_routes_[edge].count(regularized_route));
      reverse_routes_[edge].erase(regularized_route);
    }

    const size_t original_distance = routes_[regularized_route].size();
    distance_sum_ -= original_distance;
    DCHECK_EQ(1, distance_and_routes_[original_distance]
                     .count(regularized_route));
    distance_and_routes_[original_distance].erase(regularized_route);
    if (distance_and_routes_[original_distance].size() == 0) {
      distance_and_routes_.erase(original_distance);
    }
    routes_[regularized_route].clear();
    DCHECK_EQ(0, distance_and_routes_[0].count(regularized_route));
    distance_and_routes_[0].insert(regularized_route);
  }

  void RemoveRoutesForEdge(const Route& edge) {
    vector<Route> removal_routes;
    {
      const set<Route>& routes = reverse_routes_[RegularizedRoute(edge)];
      removal_routes = vector<Route>(routes.begin(), routes.end());
    }

    for (const Route& route : removal_routes) {
      RemoveRoute(route);
    }

    for (int route_index = 0; route_index < removal_routes.size();) {
      Vertex start_vertex = removal_routes[route_index].first;

      for (; route_index < removal_routes.size() &&
             removal_routes[route_index].first == start_vertex; route_index++) {
        for (Vertex vertex : Edges(removal_routes[route_index].second)) {
          int distance = RouteSize(Edge(start_vertex, vertex));
          if (distance < 0) { continue; }
          // TODO(imos): Prioritize routes.
          UpdateRoute(Route(start_vertex, vertex),
                      Route(vertex, removal_routes[route_index].second));
        }
      }
    }
  }

  int64_t distance_sum_;
  map<int, set<Route>> distance_and_routes_;
  map<Route, set<Route>> routes_;
  map<Route, set<Route>> reverse_routes_;
};

#endif  // GRAPHGOLF_FAST_EVALUATION_GRAPH_H_
