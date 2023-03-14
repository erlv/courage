/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        std::set<ListNode*> allNodeSet;
        ListNode* iterA = headA;
        ListNode* iterB = headB;
        while (iterA != NULL) {
            auto insertA = allNodeSet.insert(iterA);
            assert(insertA.first != allNodeSet.end());
            assert(insertA.second);
            iterA = iterA->next;
        }
        while (iterB != NULL) {
            auto findB = allNodeSet.find(iterB);
            if (findB == allNodeSet.end()) {
                iterB = iterB->next;
            } else {
                return iterB;
            }
        }
        return NULL;
    }
};