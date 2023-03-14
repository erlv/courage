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
        ListNode* iterA = headA;
        while ( iterA != NULL) {
            ListNode* iterB = headB;
            while (iterB != NULL) {
                if (iterA == iterB) {
                    return iterA;
                } else {
                    iterB = iterB->next;
                }
            }
            iterA = iterA->next;
        }
        return NULL;
    }
};