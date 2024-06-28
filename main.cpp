#include<iostream>
#include<mutex>
using namespace std;

//����� ����������� �������


struct Node
{
public:
	int value;
	Node* next;
	mutex* node_mutex;
};




class FineGrainedQueue
{
private:
    Node* head;

public:
    FineGrainedQueue() : head(nullptr) {};
   
 

    // ��� � �������� ����� ������� � �������� ��������� ����������������� ����������. ��������� ����� ����� ����������� ����, ���� � �������� - �� ������� ������� �� �������
    void insertIntoMiddle(int val, int afterVal)
    {
        Node* newNode = new Node();
        newNode->value = val;
        newNode->node_mutex = new std::mutex();

        // ���� ������� �����, ����� ���� ���������� �������
        if (head == nullptr) {
            head = newNode;
            return;
        }

        Node* current = head;
        std::mutex* prev_mutex = nullptr;

        // ��������� ������ ����
        std::lock_guard<std::mutex> head_lock(*head->node_mutex);

        // ���� ����, ����� �������� ����� �������� ����� ����
        while (current != nullptr && current->value != afterVal) {
            if (prev_mutex != nullptr) {
                // ������������ ���������� ����
                prev_mutex->unlock();
            }
            // ��������� ������� ����
            current->node_mutex->lock();
            prev_mutex = current->node_mutex;
            current = current->next;
        }

        // ���� �� ����� ����, ����� �������� ����� �������� ����� ����
        if (current != nullptr) {
            // ��������� ����� ����
            std::lock_guard<std::mutex> newNode_lock(*newNode->node_mutex);
            // ��������� ����� ����
            newNode->next = current->next;
            current->next = newNode;
        }

        // ������������ ��������� ��������������� ����
        if (prev_mutex != nullptr) {
            prev_mutex->unlock();
        }
    }


};




int main()
{
    setlocale(LC_ALL, "ru");

    FineGrainedQueue queue;

   

    // ��������� ������ ���� ����� ������� ����
    queue.insertIntoMiddle(3, 2);
    cout << "�������� ���� ������� ������� � ��������" << endl;







	return 0;
}