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
        int lenA = 0;
        int lenB = 0;
        ListNode* tempheadA = headA;
        ListNode* tempheadB = headB;
        while( tempheadA!= NULL) {
            lenA++;
            tempheadA= tempheadA->next;
        }
        while( tempheadB!= NULL) {
            lenB++;
            tempheadB= tempheadB->next;
        }
        tempheadA=headA;
        tempheadB=headB;
        if (lenA > lenB) {
            for (int i=0; i < (lenA-lenB); i++) {
                tempheadA = tempheadA->next;
            }
        } else {
            for (int i=0; i < (lenB - lenA); i++) {
                tempheadB = tempheadB->next;
            }
        }
        while (tempheadA != NULL && tempheadB != NULL && tempheadA != tempheadB ) {
            tempheadA = tempheadA->next;
            tempheadB = tempheadB->next;
        }
        return tempheadA==tempheadB? tempheadA:NULL;
    }
};