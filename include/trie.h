#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <string>
#include <set>

template<typename T>
class Trie {
public:
    Trie<T>();

    ~Trie() = default;

    Trie(const Trie<T>& orig) = default;

    /**
     * \brief The function of inserting into the prefix tree.
     * \param [in] s Inserted string s.
     * \return       Index of the string s in the prefix tree.
     */
    size_t insert(const std::basic_string<T>& s);

    /**
     * \brief Calculation of the maximum degree of the vertices of the prefix tree
     *        (the root of the tree is not taken into account).
     * \return The maximum degree of the vertices of the prefix tree
     *         (the root of the tree is not taken into account)
     */
    size_t maximal_degree();
protected:
    /**
     * \struct node
     * \brief Node type of the prefix tree.
     * \details All child nodes of the current node are organized in the form of a
     *          simply-connected list, the first element of which is an element with
     *          the index first_child. The  field parent contains the index of the
     *          parent node, and in the next field the next descendant of the parent
     *          node. If the current node has no children, then the field  first_child
     *          contains zero. Similarly, the last element in the list of children in
     *          the  field next contains zero. Here, the subscript is the index in the
     *          field node_buffer, which is a vector (in the sense of the STL library)
     *          of the nodes of the prefix tree.
     */
    struct node{
      size_t parent, first_child, next;

      /// \brief The length of the path from the current node to the root of the tree.
      size_t path_len;

      /// \brief The degree of the node, that is, the number of edges emerging
      /// from the node
      size_t degree;

      /// \brief The character of the inserted string that is the
      /// label of the current node
      T c;

      node(){
        next = parent = path_len = first_child = 0;
        degree = 0; c = T();
      }
    };

    std::vector<node>   node_buffer;
    std::vector<size_t> nodes_indeces;

    /**
     * \brief This function adds a node marked with a value of x of type T to the list of
     *        children of the node parent_idx.
     * \param [in] parent_idx An index of a parent.
     * \param [in] x          An inserted value.
     * \return                The index of inserted node.
     */
    size_t add_child(size_t parent_idx, T x);

    /// \brief This function performs (possibly necessary) actions after the last
    /// character is inserted.
    virtual void post_action(const std::basic_string<T>& s, size_t n){ };
};

template<typename T>
Trie<T>::Trie(){
    node_buffer = std::vector<node>(1);
    nodes_indeces = std::vector<size_t>();
}

template<typename T>
size_t Trie<T>::maximal_degree(){
    size_t deg = 0;
    size_t len = node_buffer.size();
    for(int i = 1; i < len; i++){
        deg = std::max(deg,node_buffer[i].degree);
    }
    return deg;
}

template<typename T>
size_t Trie<T>::add_child(size_t parent_idx, T x){
    size_t current, previous;
    node   temp;
    current = previous = node_buffer[parent_idx].first_child;
    /* The variable temp contains a node that you might need to insert. */
    temp.c = x; temp.degree = 0;
    temp.next = 0; temp.parent = parent_idx;
    temp.path_len = node_buffer[parent_idx].path_len + 1;
    if(!current){
        /* We can be here only if the node with the parent_idx index has no children at
         * all. This means that the added node will be the first in the list of children.
         * In this case the degree of node parent_idx will increase by one, and will
         * become equal to 1.
         */
        node_buffer.push_back(temp);
        size_t child_idx = node_buffer.size() - 1;
        node_buffer[parent_idx].first_child = child_idx;
        node_buffer[parent_idx].degree = 1;
        return child_idx;
    }
    while(current){
        // If there are children, then you need to go through the list of children.
        node current_node = node_buffer[current];
        if(current_node.c == x){
          /* If there is a child marked with the desired symbol (the symbol x),
           * then we need to return the index of this child. */
          return current;
        }else{
          previous = current; current = current_node.next;
        }
    }
    /* If there is no such child, then we need to add this child to the end
     * of the list of children. */
    node_buffer.push_back(temp);
    size_t next_child = node_buffer.size() - 1;
    node_buffer[previous].next = next_child;
    node_buffer[parent_idx].degree++;
    return next_child;
}

template<typename T>
size_t Trie<T>::insert(const std::basic_string<T>& s){
    ssize_t len = s.length();
    size_t current_root = 0;
    for (ssize_t i = 0; i < len; i++) {
        current_root = add_child(current_root,s[i]);
    }
    nodes_indeces.push_back(current_root);
    post_action(s,current_root);
    return current_root;
}
#endif
