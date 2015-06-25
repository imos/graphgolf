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

  Score EvaluateAll() const {
    int64_t distance_sum = 0;
    int distance_max = 0;
    vector<int> vertices(order());
    vector<int> distances(order(), -1);

    for (int start_vertex = 0; start_vertex < order(); start_vertex++) {
      std::fill(distances.begin(), distances.end(), -1);

      // Set the start_vertex.
      vertices[0] = start_vertex;
      distances[start_vertex] = 0;

      int read_index = 0;
      int write_index = 1;
      for (; read_index < vertices.size(); read_index++) {
        if (read_index >= write_index) {
          distance_max = order();
          break;
        }

        // The current vertex.
        const int vertex = vertices[read_index];
        // The distance from the start vertex for the vertices connected from
        // the current vertex.
        const int next_distance = distances[vertex] + 1;

        for (Vertex next_vertex : Edges(vertex)) {
          if (distances[next_vertex] != -1) continue;
          distances[next_vertex] = next_distance;
          distance_sum += next_distance;
          vertices[write_index++] = next_vertex;
        }
      }

      for (Vertex end_vertex = 0; end_vertex < order(); end_vertex++) {
        if (start_vertex == end_vertex) continue;
        int distance = distances[end_vertex];
        auto route = routes_.find(RegularizedRoute(Route(start_vertex, end_vertex)));
        int new_distance = -1;
        if (route != routes_.end() && route->second.size() > 0) {
          new_distance = route->second.size();
        }
        CHECK_EQ(distance, new_distance)
            << start_vertex << " => " << end_vertex;
      }

      distance_max = max(distance_max, distances[write_index - 1]);
    }
    return Score(distance_max, distance_sum);
  }

  Score Evaluate() const override {
    if (distance_and_routes_.count(0) > 0) {
      return Score(order(), 0);
    }

    Score evaluated_score = Graph::Evaluate();
    if (evaluated_score != Score(distance_and_routes_.rbegin()->first, distance_sum_ * 2)) {
      LOG(INFO) << "evaluated_score: " << evaluated_score.first << ", "
                << evaluated_score.second;
      LOG(INFO) << "score: " << distance_and_routes_.rbegin()->first << ", "
                << distance_sum_ * 2;
      for (auto distance_and_routes : distance_and_routes_) {
        LOG(INFO) << distance_and_routes.first;
        for (Route route : distance_and_routes.second) {
          LOG(INFO) << "route: " << route.first << " => " << route.second;
        }
      }
      LOG(FATAL) << "failed to evaluate.";
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
