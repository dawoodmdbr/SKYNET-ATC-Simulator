#ifndef AVL_H
#define AVL_H

#include <string>

struct LogEntry {
    long timestamp;
    std::string flightId;
    std::string status;
    std::string notes;
    
    LogEntry() : timestamp(0) {}
    LogEntry(long ts, const std::string& id, const std::string& st, const std::string& nt)
        : timestamp(ts), flightId(id), status(st), notes(nt) {}
};

struct AVLNode {
    LogEntry entry;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(const LogEntry& e) : entry(e), left(nullptr), right(nullptr), height(1) {}
};

class AVL {
private:
    AVLNode* root;
    
    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* insertHelper(AVLNode* node, const LogEntry& entry);
    void inOrderHelper(AVLNode* node);
    void clearHelper(AVLNode* node);
    void collectLogsHelper(AVLNode* node, LogEntry* logs, int& count, int maxCount);
    
public:
    AVL();
    ~AVL();
    
    void insert(long timestamp, const std::string& flightId, const std::string& status, const std::string& notes);
    void inOrderPrint();
    void clear();
    void collectLogs(LogEntry* logs, int& count, int maxCount);
};

#endif

