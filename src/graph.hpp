//
//  dgraph.hpp
//

#ifndef dgraph_hpp
#define dgraph_hpp

#include <cstdio>
#include <cstdint>
#include <vector>
#include <utility>
#include <functional>
#include "dna.hpp"
#include "handle.hpp"
//#include "handle_types.hpp"
//#include "handle_helper.hpp"
#include "dynamic.hpp"
#include "dynamic_types.hpp"
#include "hash_map.hpp"

namespace dg {

class graph_t : public MutablePathDeletableHandleGraph {
        
public:
    graph_t(void) {
        // set up initial delimiters
        seq_bv.push_back(1);
        edge_fwd_iv.push_back(0);
        edge_fwd_bv.push_back(1);
        edge_fwd_inv_bv.push_back(0);
        edge_rev_iv.push_back(0);
        edge_rev_bv.push_back(1);
        edge_rev_inv_bv.push_back(0);
        path_handle_wt.push_back(0);
        path_rev_iv.push_back(0);
        path_next_id_iv.push_back(0);
        path_next_rank_iv.push_back(0);
        path_prev_id_iv.push_back(0);
        path_prev_rank_iv.push_back(0);
    }

    ~graph_t(void) { clear(); }

    /// Copy constructor.
    graph_t(const graph_t& other) {
        _max_node_id = other._max_node_id;
        _min_node_id = other._min_node_id;
        _node_count = other._node_count;
        _edge_count = other._edge_count;
        _path_count = other._path_count;
        _path_handle_next = other._path_handle_next;
        graph_id_pv = other.graph_id_pv;
        graph_id_map = other.graph_id_map;
        edge_fwd_iv = other.edge_fwd_iv;
        edge_fwd_bv = other.edge_fwd_bv;
        edge_fwd_inv_bv = other.edge_fwd_inv_bv;
        edge_rev_iv = other.edge_rev_iv;
        edge_rev_bv = other.edge_rev_bv;
        edge_rev_inv_bv = other.edge_rev_inv_bv;
        seq_pv = other.seq_pv;
        seq_bv = other.seq_bv;
        path_handle_wt = other.path_handle_wt;
        path_rev_iv = other.path_rev_iv;
        path_next_id_iv = other.path_next_id_iv;
        path_next_rank_iv = other.path_next_rank_iv;
        path_prev_id_iv = other.path_prev_id_iv;
        path_prev_rank_iv = other.path_prev_rank_iv;
        path_metadata_map = other.path_metadata_map;
        path_name_map = other.path_name_map;
    }

    /// Move constructor.
    graph_t(graph_t&& other) noexcept {
        _max_node_id = other._max_node_id;
        _min_node_id = other._min_node_id;
        _node_count = other._node_count;
        _edge_count = other._edge_count;
        _path_count = other._path_count;
        _path_handle_next = other._path_handle_next;
        graph_id_pv = other.graph_id_pv;
        graph_id_map = other.graph_id_map;
        edge_fwd_iv = other.edge_fwd_iv;
        edge_fwd_bv = other.edge_fwd_bv;
        edge_fwd_inv_bv = other.edge_fwd_inv_bv;
        edge_rev_iv = other.edge_rev_iv;
        edge_rev_bv = other.edge_rev_bv;
        edge_rev_inv_bv = other.edge_rev_inv_bv;
        seq_pv = other.seq_pv;
        seq_bv = other.seq_bv;
        path_handle_wt = other.path_handle_wt;
        path_rev_iv = other.path_rev_iv;
        path_next_id_iv = other.path_next_id_iv;
        path_next_rank_iv = other.path_next_rank_iv;
        path_prev_id_iv = other.path_prev_id_iv;
        path_prev_rank_iv = other.path_prev_rank_iv;
        path_metadata_map = other.path_metadata_map;
        path_name_map = other.path_name_map;
    }

    /// Copy assignment operator.
    graph_t& operator=(const graph_t& other) {
        graph_t tmp(other);
        *this = std::move(tmp);
        return *this;
    }

    /// Move assignment operator.
    graph_t& operator=(graph_t&& other) noexcept {
        // todo should we use std::swap ?
        _max_node_id = other._max_node_id;
        _min_node_id = other._min_node_id;
        _node_count = other._node_count;
        _edge_count = other._edge_count;
        _path_count = other._path_count;
        _path_handle_next = other._path_handle_next;
        graph_id_pv = other.graph_id_pv;
        graph_id_map = other.graph_id_map;
        edge_fwd_iv = other.edge_fwd_iv;
        edge_fwd_bv = other.edge_fwd_bv;
        edge_fwd_inv_bv = other.edge_fwd_inv_bv;
        edge_rev_iv = other.edge_rev_iv;
        edge_rev_bv = other.edge_rev_bv;
        edge_rev_inv_bv = other.edge_rev_inv_bv;
        seq_pv = other.seq_pv;
        seq_bv = other.seq_bv;
        path_handle_wt = other.path_handle_wt;
        path_rev_iv = other.path_rev_iv;
        path_next_id_iv = other.path_next_id_iv;
        path_next_rank_iv = other.path_next_rank_iv;
        path_prev_id_iv = other.path_prev_id_iv;
        path_prev_rank_iv = other.path_prev_rank_iv;
        path_metadata_map = other.path_metadata_map;
        path_name_map = other.path_name_map;
        return *this;
    }

    /// Method to check if a node exists by ID
    bool has_node(id_t node_id) const;
    
    /// Look up the handle for the node with the given ID in the given orientation
    handle_t get_handle(const id_t& node_id, bool is_reverse = false) const;
    
    /// Get the ID from a handle
    id_t get_id(const handle_t& handle) const;
    
    /// Get the orientation of a handle
    bool get_is_reverse(const handle_t& handle) const;
    
    /// Invert the orientation of a handle (potentially without getting its ID)
    handle_t flip(const handle_t& handle) const;
    
    /// Get the length of a node
    size_t get_length(const handle_t& handle) const;
    
    /// Get the sequence of a node, presented in the handle's local forward orientation.
    std::string get_sequence(const handle_t& handle) const;
    
    /// Loop over all the handles to next/previous (right/left) nodes. Passes
    /// them to a callback which returns false to stop iterating and true to
    /// continue. Returns true if we finished and false if we stopped early.
    bool follow_edges(const handle_t& handle, bool go_left, const std::function<bool(const handle_t&)>& iteratee) const;
    
    /// Loop over all the nodes in the graph in their local forward
    /// orientations, in their internal stored order. Stop if the iteratee
    /// returns false. Can be told to run in parallel, in which case stopping
    /// after a false return value is on a best-effort basis and iteration
    /// order is not defined.
    void for_each_handle(const std::function<bool(const handle_t&)>& iteratee, bool parallel = false) const;
    
    /// Return the number of nodes in the graph
    /// TODO: can't be node_count because XG has a field named node_count.
    size_t node_size(void) const;
    
    /// Return the smallest ID in the graph, or some smaller number if the
    /// smallest ID is unavailable. Return value is unspecified if the graph is empty.
    id_t min_node_id(void) const;
    
    /// Return the largest ID in the graph, or some larger number if the
    /// largest ID is unavailable. Return value is unspecified if the graph is empty.
    id_t max_node_id(void) const;
    
    ////////////////////////////////////////////////////////////////////////////
    // Interface that needs to be using'd
    ////////////////////////////////////////////////////////////////////////////
    
    /// Loop over all the handles to next/previous (right/left) nodes. Works
    /// with a callback that just takes all the handles and returns void.
    /// Has to be a template because we can't overload on the types of std::function arguments otherwise.
    /// MUST be pulled into implementing classes with `using` in order to work!
    template <typename T>
    auto follow_edges(const handle_t& handle, bool go_left, T&& iteratee) const
        -> typename std::enable_if<std::is_void<decltype(iteratee(get_handle(0, false)))>::value>::type {
        // Implementation only for void-returning iteratees
        // We ought to just overload on the std::function but that's not allowed until C++14.
        // See <https://stackoverflow.com/q/13811180>
        
        // We also can't use result_of<T(handle_t)>::type to sniff the return
        // type out because that ::type would not exist (since that's what you
        // get for a void apparently?) and we couldn't check if it's bool or
        // void.
        
        // So we do this nonsense thing with a trailing return type (to get the
        // actual arg into scope) and a decltype (which is allowed to resolve to
        // void) and is_void (which is allowed to take void) and a fake
        // get_handle call (which is the shortest handle_t-typed expression I
        // could think of).
        
        // Make a wrapper that puts a bool return type on.
        std::function<bool(const handle_t&)> lambda = [&](const handle_t& found) {
            iteratee(found);
            return true;
        };
        
        // Use that
        follow_edges(handle, go_left, lambda);
        
        // During development I managed to get earlier versions of this template to build infinitely recursive functions.
        static_assert(!std::is_void<decltype(lambda(get_handle(0, false)))>::value, "can't take our own lambda");
    }
    
    /// Loop over all the nodes in the graph in their local forward
    /// orientations, in their internal stored order. Works with void-returning iteratees.
    /// MUST be pulled into implementing classes with `using` in order to work!
    template <typename T>
    auto for_each_handle(T&& iteratee, bool parallel = false) const
    -> typename std::enable_if<std::is_void<decltype(iteratee(get_handle(0, false)))>::value>::type {
        // Make a wrapper that puts a bool return type on.
        std::function<bool(const handle_t&)> lambda = [&](const handle_t& found) {
            iteratee(found);
            return true;
        };
        
        // Use that
        for_each_handle(lambda, parallel);
    }

    void for_each_edge(const std::function<bool(const edge_t&)>& iteratee, bool parallel = false);
    
    /// Get a handle from a Visit Protobuf object.
    /// Must be using'd to avoid shadowing.
    //handle_t get_handle(const Visit& visit) const;
    
    ////////////////////////////////////////////////////////////////////////////
    // Additional optional interface with a default implementation
    ////////////////////////////////////////////////////////////////////////////
    
    /// Get the number of edges on the right (go_left = false) or left (go_left
    /// = true) side of the given handle. The default implementation is O(n) in
    /// the number of edges returned, but graph implementations that track this
    /// information more efficiently can override this method.
    size_t get_degree(const handle_t& handle, bool go_left) const;
    
    ////////////////////////////////////////////////////////////////////////////
    // Concrete utility methods
    ////////////////////////////////////////////////////////////////////////////
    
    /// Get a Protobuf Visit from a handle.
    //Visit to_visit(const handle_t& handle) const;
    
    /// Get the locally forward version of a handle
    handle_t forward(const handle_t& handle) const;
    
    /// A pair of handles can be used as an edge. When so used, the handles have a
    /// canonical order and orientation.
    edge_t edge_handle(const handle_t& left, const handle_t& right) const;
    
    /// Such a pair can be viewed from either inward end handle and produce the
    /// outward handle you would arrive at.
    handle_t traverse_edge_handle(const edge_t& edge, const handle_t& left) const;
    
/**
 * This is the interface for a handle graph that stores embedded paths.
 */
    
    ////////////////////////////////////////////////////////////////////////////
    // Path handle interface that needs to be implemented
    ////////////////////////////////////////////////////////////////////////////
    
    /// Determine if a path name exists and is legal to get a path handle for.
    bool has_path(const std::string& path_name) const;
    
    /// Look up the path handle for the given path name.
    /// The path with that name must exist.
    path_handle_t get_path_handle(const std::string& path_name) const;
    
    /// Look up the name of a path from a handle to it
    std::string get_path_name(const path_handle_t& path_handle) const;
    
    /// Returns the number of node occurrences in the path
    size_t get_occurrence_count(const path_handle_t& path_handle) const;

    /// Returns the number of paths stored in the graph
    size_t get_path_count(void) const;

    /// Execute a function on each path in the graph
    // TODO: allow stopping early?
    void for_each_path_handle(const std::function<void(const path_handle_t&)>& iteratee) const;

    /// Returns a vector of all occurrences of a node on paths. Optionally restricts to
    /// occurrences that match the handle in orientation.
    std::vector<occurrence_handle_t> occurrences_of_handle(const handle_t& handle,
                                                           bool match_orientation = false) const;
    
    /// Enumerate the path occurrences on a given handle (strand agnostic) *TODO move to vg
    void for_each_occurrence_on_handle(const handle_t& handle, const std::function<void(const occurrence_handle_t&)>& iteratee) const;

    /// Returns the number of node occurrences on the handle
    size_t get_occurrence_count(const handle_t& handle) const;
    
    /// Get a node handle (node ID and orientation) from a handle to an occurrence on a path
    handle_t get_occurrence(const occurrence_handle_t& occurrence_handle) const;

    /// Get a path handle (path ID) from a handle to an occurrence on a path
    path_handle_t get_path(const occurrence_handle_t& occurrence_handle) const;
    
    /// Get a handle to the first occurrence in a path.
    /// The path MUST be nonempty.
    occurrence_handle_t get_first_occurrence(const path_handle_t& path_handle) const;
    
    /// Get a handle to the last occurrence in a path
    /// The path MUST be nonempty.
    occurrence_handle_t get_last_occurrence(const path_handle_t& path_handle) const;
    
    /// Returns true if the occurrence is not the last occurence on the path, else false
    bool has_next_occurrence(const occurrence_handle_t& occurrence_handle) const;
    
    /// Returns true if the occurrence is not the first occurence on the path, else false
    bool has_previous_occurrence(const occurrence_handle_t& occurrence_handle) const;
    
    /// Returns a handle to the next occurrence on the path
    occurrence_handle_t get_next_occurrence(const occurrence_handle_t& occurrence_handle) const;
    
    /// Returns a handle to the previous occurrence on the path
    occurrence_handle_t get_previous_occurrence(const occurrence_handle_t& occurrence_handle) const;
    
    /// Returns a handle to the path that an occurrence is on
    path_handle_t get_path_handle_of_occurrence(const occurrence_handle_t& occurrence_handle) const;
    
    /// Returns the 0-based ordinal rank of a occurrence on a path
    size_t get_ordinal_rank_of_occurrence(const occurrence_handle_t& occurrence_handle) const;

    ////////////////////////////////////////////////////////////////////////////
    // Additional optional interface with a default implementation
    ////////////////////////////////////////////////////////////////////////////

    /// Returns true if the given path is empty, and false otherwise
    bool is_empty(const path_handle_t& path_handle) const;

    ////////////////////////////////////////////////////////////////////////////
    // Concrete utility methods
    ////////////////////////////////////////////////////////////////////////////

    /// Loop over all the occurrences along a path, from first through last
    void for_each_occurrence_in_path(const path_handle_t& path, const std::function<void(const occurrence_handle_t&)>& iteratee) const;

/**
 * This is the interface for a handle graph that supports modification.
 */
    /*
     * Note: All operations may invalidate path handles and occurrence handles.
     */
    
    /// Create a new node with the given sequence and return the handle.
    handle_t create_handle(const std::string& sequence);

    /// Create a new node with the given id and sequence, then return the handle.
    handle_t create_handle(const std::string& sequence, const id_t& id);

    /// Create a "hidden" node which might carry parts of paths that traversed deleted portions of the graph
    handle_t create_hidden_handle(const std::string& sequence);
    
    /// Remove the node belonging to the given handle and all of its edges.
    /// Does not update any stored paths.
    /// Invalidates the destroyed handle.
    /// May be called during serial for_each_handle iteration **ONLY** on the node being iterated.
    /// May **NOT** be called during parallel for_each_handle iteration.
    /// May **NOT** be called on the node from which edges are being followed during follow_edges.
    void destroy_handle(const handle_t& handle);
    
    /// Create an edge connecting the given handles in the given order and orientations.
    /// Ignores existing edges.
    void create_edge(const handle_t& left, const handle_t& right);

    /// Check if an edge exists
    bool has_edge(const handle_t& left, const handle_t& right) const;

    /// Convenient wrapper for create_edge.
    inline void create_edge(const edge_t& edge) {
        create_edge(edge.first, edge.second);
    }
    
    /// Remove the edge connecting the given handles in the given order and orientations.
    /// Ignores nonexistent edges.
    /// Does not update any stored paths.
    void destroy_edge(const handle_t& left, const handle_t& right);
    
    /// Convenient wrapper for destroy_edge.
    inline void destroy_edge(const edge_t& edge) {
        destroy_edge(edge.first, edge.second);
    }
    
    /// Remove all nodes and edges. Does not update any stored paths.
    void clear(void);
    
    /// Swap the nodes corresponding to the given handles, in the ordering used
    /// by for_each_handle when looping over the graph. Other handles to the
    /// nodes being swapped must not be invalidated. If a swap is made while
    /// for_each_handle is running, it affects the order of the handles
    /// traversed during the current traversal (so swapping an already seen
    /// handle to a later handle's position will make the seen handle be visited
    /// again and the later handle not be visited at all).
    void swap_handles(const handle_t& a, const handle_t& b);
    
    /// Alter the node that the given handle corresponds to so the orientation
    /// indicated by the handle becomes the node's local forward orientation.
    /// Rewrites all edges pointing to the node and the node's sequence to
    /// reflect this. Invalidates all handles to the node (including the one
    /// passed). Returns a new, valid handle to the node in its new forward
    /// orientation. Note that it is possible for the node's ID to change.
    /// Does not update any stored paths. May change the ordering of the underlying
    /// graph.
    handle_t apply_orientation(const handle_t& handle);
    
    /// Split a handle's underlying node at the given offsets in the handle's
    /// orientation. Returns all of the handles to the parts. Other handles to
    /// the node being split may be invalidated. The split pieces stay in the
    /// same local forward orientation as the original node, but the returned
    /// handles come in the order and orientation appropriate for the handle
    /// passed in.
    /// Updates stored paths.
    std::vector<handle_t> divide_handle(const handle_t& handle, const std::vector<size_t>& offsets);
    
    /// Specialization of divide_handle for a single division point
    inline std::pair<handle_t, handle_t> divide_handle(const handle_t& handle, size_t offset) {
        auto parts = divide_handle(handle, std::vector<size_t>{offset});
        return std::make_pair(parts.front(), parts.back());
    }

/**
 * This is the interface for a handle graph with embedded paths where the paths can be modified.
 * Note that if the *graph* can also be modified, the implementation will also
 * need to inherit from MutableHandleGraph, via the combination
 * MutablePathMutableHandleGraph interface.
 * TODO: This is a very limited interface at the moment. It will probably need to be extended.
 */
    
    /**
     * Destroy the given path. Invalidates handles to the path and its node occurrences.
     */
    void destroy_path(const path_handle_t& path);

    /**
     * Create a path with the given name. The caller must ensure that no path
     * with the given name exists already, or the behavior is undefined.
     * Returns a handle to the created empty path. Handles to other paths must
     * remain valid.
     */
    path_handle_t create_path_handle(const std::string& name);
    
    /**
     * Append a visit to a node to the given path. Returns a handle to the new
     * final occurrence on the path which is appended. Handles to prior
     * occurrences on the path, and to other paths, must remain valid.
     */
    occurrence_handle_t append_occurrence(const path_handle_t& path, const handle_t& to_append);

    /**
     * Insert a visit to a node to the given path between the given occurrences.
     * Returns a handle to the new occurrence on the path which is appended.
     * Handles to prior occurrences on the path, and to other paths, must remain valid.
     */
    occurrence_handle_t insert_occurrence(const occurrence_handle_t& before, const occurrence_handle_t& after, const handle_t& to_insert);
    
    /// Set the occurrence to the given handle, possibly re-linking and cleaning up if needed
    occurrence_handle_t set_occurrence(const occurrence_handle_t& occurrence_handle, const handle_t& handle);

    /// Replace the occurrence with multiple handles
    std::vector<occurrence_handle_t> replace_occurrence(const occurrence_handle_t& occurrence_handle, const std::vector<handle_t>& handles);

    /// A helper function to visualize the state of the graph
    void display(void) const;

    /// Convert to GFA (for debugging)
    void to_gfa(std::ostream& out) const;

    /// Serialize
    uint64_t serialize(std::ostream& out) const;

    /// Load
    void load(std::istream& in);
    
/// These are the backing data structures that we use to fulfill the above functions

private:

    /// Records node ids to allow for random access and random order
    /// Use the special value "0" to indicate deleted nodes
    dyn::packed_vector graph_id_pv;
    /// efficient id to handle conversion
    hash_map<uint64_t, uint64_t> graph_id_map;
    id_t _max_node_id = 0;
    id_t _min_node_id = 0;
    /// records nodes that are hidden, but used to store path sequence that has been removed from the node space
    hash_set<uint64_t> graph_id_hidden_set;

    /// Records edges of the 3' end on the forward strand, delimited by 0
    /// ordered by rank in graph_id_wt, recorded by δ = id_this - id_that
    /// and stored as (δ = 0 ? 1 : (δ > 0 ? 2δ+1 : 2δ+2))
    lciv_iv edge_fwd_iv;
    suc_bv edge_fwd_bv;

    /// Marks inverting edges in edge_fwd_wt
    suc_bv edge_fwd_inv_bv;

    /// Records edges of the 3' end on the reverse strand, delimited by 0,
    /// ordered by rank in graph_id_wt, recorded by δ = id_this - id_that
    /// and stored as (δ = 0 ? 1 : (δ > 0 ? 2δ+1 : 2δ+2))
    lciv_iv edge_rev_iv;
    suc_bv edge_rev_bv;

    /// Marks inverting edges in edge_rev_wt
    suc_bv edge_rev_inv_bv;

    /// Encodes all of the sequences of all nodes and all paths in the graph.
    /// The node sequences occur in the same order as in graph_iv;
    /// Node boundaries are given by 0s
    dyn::packed_vector seq_pv;

    /// Marks the beginnings of nodes in seq_pv
    suc_bv seq_bv;

    /// ordered path identifiers as they traverse each node
    /// the index of the path identifier in this WT defines the occurrence_handle_t for the step
    /// which also maps into the path_next_* and path_prev_* WTs
    wt_str path_handle_wt;

    /// which orientation are we traversing in?
    lciv_iv path_rev_iv;

    /// special delimiters used in the path_next_id_wt and path_prev_id_wt
    const static uint64_t path_begin_marker = 1; //std::numeric_limits<uint64_t>::max()-1;
    const static uint64_t path_end_marker = 2; // std::numeric_limits<uint64_t>::max();
    
    /// by occurrence, where this particular occurrence goes next
    lciv_iv path_next_id_iv;

    /// the rank of the path occurrence among occurrences in this path in the next handle's list
    lciv_iv path_next_rank_iv;

    /// by occurrence, where this particular occurrence came from 
    lciv_iv path_prev_id_iv;

    /// the rank of the path occurrence among occurrences in this path in the previous handle's list
    lciv_iv path_prev_rank_iv;

    struct path_metadata_t {
        uint64_t length;
        occurrence_handle_t first;
        occurrence_handle_t last;
        std::string name;
    };
    /// maps between path identifier and the start, end, and length of the path
    hash_map<uint64_t, path_metadata_t> path_metadata_map;

    /// Links path names to handles
    string_hash_map<std::string, uint64_t> path_name_map;

    /// A helper to record the number of live nodes
    uint64_t _node_count = 0;

    /// A counter that records the number of hidden nodes
    uint64_t _hidden_count = 0;

    /// A helper to record the number of live edges
    uint64_t _edge_count = 0;

    /// A helper to record the number of live paths
    uint64_t _path_count = 0;

    /// A helper to record the next path handle (path deletions are hard because of our path FM-index)
    uint64_t _path_handle_next = 0;

    /// Helper to convert between edge storage and actual id
    uint64_t edge_delta_to_id(uint64_t left, uint64_t delta) const;

    /// Helper to convert between ids and stored edge
    uint64_t edge_to_delta(const handle_t& left, const handle_t& right) const;

    /// Helper to simplify removal of path handle records
    void destroy_path_handle_records(uint64_t i);

    /// Helper to create the internal records for the occurrence
    occurrence_handle_t create_occurrence(const path_handle_t& path, const handle_t& handle);

    /// Helper to destroy the internal records for the occurrence
    void destroy_occurrence(const occurrence_handle_t& occurrence_handle);

    /// Helper to stitch up partially built paths
    void link_occurrences(const occurrence_handle_t& from, const occurrence_handle_t& to);

    /// Decrement the occurrence rank references for this occurrence
    void decrement_rank(const occurrence_handle_t& occurrence_handle);

    /// The internal rank of the occurrence
    uint64_t occurrence_rank(const occurrence_handle_t& occurrence_handle) const;

};

} // end dankness

#endif /* dgraph_hpp */
