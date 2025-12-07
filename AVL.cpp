#include "AVL.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

AVL::AVL() : root(nullptr) {
}

AVL::~AVL() {
    clear();
}

int AVL::getHeight(AVLNode* node) {
    if (node == nullptr) return 0;
    return node->height;
}

int AVL::getBalance(AVLNode* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode* AVL::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    
    return x;
}

AVLNode* AVL::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    
    return y;
}

AVLNode* AVL::insertHelper(AVLNode* node, const LogEntry& entry) {
    if (node == nullptr) {
        return new AVLNode(entry);
    }
    
    if (entry.timestamp < node->entry.timestamp) {
        node->left = insertHelper(node->left, entry);
    } else if (entry.timestamp > node->entry.timestamp) {
        node->right = insertHelper(node->right, entry);
    } else {
        return node;
    }
    
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    
    int balance = getBalance(node);
    
    if (balance > 1 && entry.timestamp < node->left->entry.timestamp) {
        return rotateRight(node);
    }
    
    if (balance < -1 && entry.timestamp > node->right->entry.timestamp) {
        return rotateLeft(node);
    }
    
    if (balance > 1 && entry.timestamp > node->left->entry.timestamp) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    if (balance < -1 && entry.timestamp < node->right->entry.timestamp) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

void AVL::insert(long timestamp, const std::string& flightId, const std::string& status, const std::string& notes) {
    LogEntry entry(timestamp, flightId, status, notes);
    root = insertHelper(root, entry);
}

void AVL::inOrderHelper(AVLNode* node) {
    if (node == nullptr) return;
    
    inOrderHelper(node->left);
    std::cout << std::setw(15) << node->entry.timestamp 
              << std::setw(15) << node->entry.flightId
              << std::setw(20) << node->entry.status
              << std::setw(30) << node->entry.notes << std::endl;
    inOrderHelper(node->right);
}

void AVL::inOrderPrint() {
    std::cout << std::endl << "=== Flight Logs ===" << std::endl;
    std::cout << std::setw(15) << "Timestamp" 
              << std::setw(15) << "Flight ID"
              << std::setw(20) << "Status"
              << std::setw(30) << "Notes" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    inOrderHelper(root);
    std::cout << std::endl;
}

void AVL::clearHelper(AVLNode* node) {
    if (node == nullptr) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
}

void AVL::clear() {
    clearHelper(root);
    root = nullptr;
}

void AVL::collectLogsHelper(AVLNode* node, LogEntry* logs, int& count, int maxCount) {
    if (node == nullptr || count >= maxCount) return;
    collectLogsHelper(node->left, logs, count, maxCount);
    if (count < maxCount) {
        logs[count++] = node->entry;
    }
    collectLogsHelper(node->right, logs, count, maxCount);
}

void AVL::collectLogs(LogEntry* logs, int& count, int maxCount) {
    count = 0;
    collectLogsHelper(root, logs, count, maxCount);
}

