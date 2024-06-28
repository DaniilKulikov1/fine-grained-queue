#include<iostream>
#include<mutex>
using namespace std;

//начнём реализовать очередь


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
   
 

    // тут я реализую метод вставки в середину используя мелкограннулярную блокировку. Поскольку метод нужно реализовать один, один и реализую - не успеваю закрыть всё вовремя
    void insertIntoMiddle(int val, int afterVal)
    {
        Node* newNode = new Node();
        newNode->value = val;
        newNode->node_mutex = new std::mutex();

        // Если очередь пуста, новый узел становится головой
        if (head == nullptr) {
            head = newNode;
            return;
        }

        Node* current = head;
        std::mutex* prev_mutex = nullptr;

        // Блокируем первый узел
        std::lock_guard<std::mutex> head_lock(*head->node_mutex);

        // Ищем узел, после которого нужно вставить новый узел
        while (current != nullptr && current->value != afterVal) {
            if (prev_mutex != nullptr) {
                // Разблокируем предыдущий узел
                prev_mutex->unlock();
            }
            // Блокируем текущий узел
            current->node_mutex->lock();
            prev_mutex = current->node_mutex;
            current = current->next;
        }

        // Если мы нашли узел, после которого нужно вставить новый узел
        if (current != nullptr) {
            // Блокируем новый узел
            std::lock_guard<std::mutex> newNode_lock(*newNode->node_mutex);
            // Вставляем новый узел
            newNode->next = current->next;
            current->next = newNode;
        }

        // Разблокируем последний заблокированный узел
        if (prev_mutex != nullptr) {
            prev_mutex->unlock();
        }
    }


};




int main()
{
    setlocale(LC_ALL, "ru");

    FineGrainedQueue queue;

   

    // добавляем третий узел после второго узла
    queue.insertIntoMiddle(3, 2);
    cout << "Добавили узел методом вставки в середину" << endl;







	return 0;
}