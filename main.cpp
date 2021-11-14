#include <stdexcept>
#include <iostream>
#include <string>

size_t GlobalTime = 0;

void print(const std::string log) {
    // std::cout << log << std::endl;
}


template<typename T>
struct linkedListItem {
    linkedListItem * next;
    T* data;
};

template<typename T>
class CLinkedList {
    public:
        ~CLinkedList() {
            print("CLinkedList destructor init");
            print(name);
            if (notToBeFreed) {
                print("notToBeFreed is true, returning.");
                return;
            }
            print("notToBeFreed is false, continuing.");
            linkedListItem<T> * tmp;

            while (this->first != nullptr) {
                tmp = this->first;
                this->first = this->first->next;
                delete tmp->data;
                print("Deleted linkedListItem Data");
                delete tmp;
                print("Deleted linkedListItem");
            }
        }

        void insertStart(const T x) {
            linkedListItem<T> * newItem = new linkedListItem<T>;

            T * tmp = new T;
            *(tmp) = x;
            newItem->data = tmp;
            newItem->next = this->first;

            this->first = newItem;

            this->itemCount++;
            notToBeFreed = false;

        }

        void insertStart(T * x, const bool pointer) {
            linkedListItem<T> * newItem = new linkedListItem<T>;
            newItem->data = x;
            newItem->next = this->first;

            this->first = newItem;

            this->itemCount++;
            notToBeFreed = false;
        }

        void insertEnd(const T x) {
            linkedListItem<T> * newItem = new linkedListItem<T>;
            T * tmp = new T;
            *(tmp) = x;
            newItem->data = tmp;
            newItem->next = nullptr;

            if (this->last)
                this->last->next = newItem;
            else {
                this->first = newItem;
            }

            this->last = newItem;

            this->itemCount++;
            notToBeFreed = false;
        }

        void insertEnd(T * x, const bool pointer) {
            linkedListItem<T> * newItem = new linkedListItem<T>;
            newItem->data = x;
            newItem->next = nullptr;

            if (this->last)
                this->last->next = newItem;
            else {
                this->first = newItem;
            }

            this->last = newItem;

            this->itemCount++;
            notToBeFreed = false;
        }

        void deleteAll(void) {
            linkedListItem<T> * tmp;

            while (this->first != nullptr) {
                tmp = this->first;
                this->first = this->first->next;
                delete tmp->data;
                delete tmp;
            }

            this->first = nullptr;
            this->last = nullptr;
            this->it = nullptr;
            this->itemCount = 0;
        }

        /**
         * Deletes all linkedListItems, but not the stored data
         */
        void deleteAll(const bool trigger) {
            linkedListItem<T> * tmp;

            while (this->first != nullptr) {
                tmp = this->first;
                this->first = this->first->next;
                if (trigger) {
                    delete tmp->data;
                    print("Deleted linkedListItem Data");
                }
                delete tmp;
                print("Deleted linkedListItem");
            }

            this->first = nullptr;
            this->last = nullptr;
            this->it = nullptr;
            this->itemCount = 0;
            notToBeFreed = true;
        }

        T * at(const unsigned n) const { // Starting with 0
            if (!itemCount || n >= itemCount)
                throw std::invalid_argument("This item does not exist in singly linked list");

            linkedListItem<T> * tmp = this->first;

            for (unsigned i = 0; i < n; i++) {
                tmp = tmp->next;
            }
            return tmp->data;
        }

        unsigned size(void) const {
            return itemCount;
        }

        T * next() {
            if (this->it == nullptr || this->it->next == nullptr)
                this->it = this->first;
            else
                this->it = this->it->next;
            return this->it->data;
        }

        void setIterator(const T* item) {
             linkedListItem<T> * tmp = this->first;

             while (tmp->data != item) {
                 tmp = tmp->next;
             }
             this->it = tmp;
        }

        void resetIterator(void) {
            it = nullptr;
        }

        void remove(const T* item) {
            linkedListItem<T> * tmp = this->first;

            if (tmp->data == item) {
                if (itemCount == 1)
                    this->last = nullptr;
                this->first = tmp->next;
                if (itemCount == 2)
                    this->last = tmp;
                delete tmp->data;
                delete tmp;
                itemCount--;
                return;
            }

            unsigned counter = 0;
            while (!(tmp->next->data == item)) {
                tmp = tmp->next;
                counter++;
            }
            linkedListItem<T> * nextnext = tmp->next->next;
            delete tmp->next->data;
            delete tmp->next;
            if (counter == this->itemCount - 2)
                tmp->next = nullptr;
            else
                tmp->next = nextnext;
            itemCount--;
        }

        void remove(const T* item, const bool deleteI) {
            linkedListItem<T> * tmp = this->first;

            if (tmp->data == item) {
                if (itemCount == 1)
                    this->last = nullptr;
                this->first = tmp->next;
                if (itemCount == 2)
                    this->last = tmp;
                if (deleteI) {
                    delete tmp->data;
                }
                delete tmp;
                itemCount--;
                return;
            }

            unsigned counter = 0;
            while (!(tmp->next->data == item)) {
                tmp = tmp->next;
                counter++;
            }
            linkedListItem<T> * nextnext = tmp->next->next;
            if (deleteI) {
                delete tmp->next->data;
            }
            delete tmp->next;
            if (counter == this->itemCount - 2)
                tmp->next = nullptr;
            else
                tmp->next = nextnext;
            itemCount--;
        }

        void rename(const std::string newName) {    // TODO delete
            name = newName;
        }

        T * find(T * data) const {
            linkedListItem<T> * tmp = this->first;

            for (size_t i = 0; i < itemCount; i++) {
                if (tmp->data == data)
                    break;
                tmp = tmp->next;
            }
            if (tmp == nullptr)
                return nullptr;
            return tmp->data;
        }

    private:
        linkedListItem<T> * first = nullptr;
        linkedListItem<T> * last = nullptr;
        linkedListItem<T> * it = nullptr;
        unsigned itemCount = 0;
        bool notToBeFreed = false;
        std::string name = "";
};


template<typename T>
struct binHeapItem {
    binHeapItem * parent = nullptr;
    binHeapItem * leftSibling = nullptr;
    binHeapItem * righestChild = nullptr;
    unsigned degree = 0;
    unsigned timeStamp;
    T data;

    binHeapItem() = default;
    binHeapItem(const T newData, const unsigned time) {
        this->data = newData;
        this->timeStamp = time;
    }
};

template<typename T>
class CBinomialHeap {
    public:
        ~CBinomialHeap() {
            for (size_t i = 0; i < rootCount(); i++) {
                destructTree(binomialTrees.next()->righestChild);
            }
        }

        binHeapItem<T> * addItem(const T x) {
            binomialTrees.insertStart(binHeapItem<T>(x, GlobalTime++));
            binHeapItem<T> * tmp = binomialTrees.at(0);
            connectEqualDegrees();
            treeCount = binomialTrees.size();
            if (min == nullptr)
                min = binomialTrees.at(0);
            else {
                if (x <= min->data)
                    min = tmp;
            }
            return tmp;
        }

        void rename(const std::string newName) {
            binomialTrees.rename(newName);
        }

        void merge(CBinomialHeap & heap) {    // TODO remake it to next()
            print("Inside merge()");
            
            CLinkedList<binHeapItem<T>> newTrees;

            mergeTwoLists(binomialTrees, heap.binomialTrees, newTrees);
        }

        binHeapItem<T> * at(const size_t index) const {
            if (!treeCount || index >= treeCount)
                throw std::invalid_argument("This item does not exist in singly linked list");
            return binomialTrees.at(index);
        }

        size_t rootCount() const {
            return binomialTrees.size();
        }

        void printAll() {
            print("Inside printAll()");
            for (size_t i = 0; i < rootCount(); i++) {
                std::cout << "Tree:" << i << std::endl;
                printTree(binomialTrees.next());
            }
        }

        void deleteAll(const bool trigger) {
            binomialTrees.deleteAll(trigger);
            treeCount = 0;
        }

        binHeapItem<T> * findMin(void) {
            return findNewMin();
        }

        void deleteBinHeapItem(binHeapItem<T> * item) {

            if (item == nullptr)
                return;
            if (binomialTrees.find(item) != nullptr) {  // is root
                std::cout << "Is root" << std::endl;    // TODO delete

                CLinkedList<binHeapItem<T>> newTrees;
                CLinkedList<binHeapItem<T>> sons;

                binHeapItem<T> * tmp = item->righestChild;
                while (tmp != nullptr) {
                    tmp->parent = nullptr;
                    sons.insertStart(tmp, true);
                    tmp = tmp->leftSibling;
                }

                binomialTrees.remove(item);
                treeCount = binomialTrees.size();

                mergeTwoLists(binomialTrees, sons, newTrees);
                
                            
            }
            else {
                CBinomialHeap tmpHeap;
                binHeapItem<T> * tmpItem = item;
                while (tmpItem->parent != nullptr) {
                    tmpItem = tmpItem->parent;
                }
                binomialTrees.remove(tmpItem, false);
                addOtherInTree(tmpHeap, tmpItem, item);
                this->merge(tmpHeap);
            }
            if (item == min)
                findNewMin();


        }

    private:
        CLinkedList<binHeapItem<T>> binomialTrees;
        size_t treeCount = 0;
        binHeapItem<T> * min = nullptr;

        void connectEqualDegrees(void) {
            print("Inside connectEqualDegrees()");
            
            bool meldHappened = true;
            while (meldHappened) {
                binomialTrees.resetIterator();
                binHeapItem<T> * tmpA = binomialTrees.next(), * tmpB;
                if (binomialTrees.size() == 1)
                    return;
                meldHappened = false;
                unsigned i = 0;
                while(i < binomialTrees.size() - 1) {
                    tmpB = binomialTrees.next();
                    if (tmpA->degree == tmpB->degree) {
                        meldHappened = true;
                        if (tmpA->data > tmpB->data || (tmpA->data == tmpB->data && tmpA->timeStamp < tmpB->timeStamp)) {
                            tmpA->leftSibling = tmpB->righestChild;
                            tmpB->righestChild = tmpA;
                            binomialTrees.remove(tmpA, false);
                            tmpA->parent = tmpB;
                            tmpB->degree += 1;
                            tmpA = binomialTrees.next();
                        }
                        else {
                            tmpB->leftSibling = tmpA->righestChild;
                            tmpA->righestChild = tmpB;
                            binomialTrees.remove(tmpB, false);
                            tmpB->parent = tmpA;
                            tmpA->degree += 1;
                            binomialTrees.setIterator(tmpA);
                            tmpA = binomialTrees.next();
                        }
                    }
                    else
                        tmpA = tmpB;
                    i++;
                }
            }
        }

        void printTree(const binHeapItem<T> * root) const {
            if (root->leftSibling != nullptr)
                printTree(root->leftSibling);
            if (root->righestChild != nullptr)
                printTree(root->righestChild);
            
            std::cout << "Degree: " << root->degree << " Data: " << root->data << std::endl;
        }

        void destructTree(binHeapItem<T> * root) const {
            if (root == nullptr)
                return;
            if (root->leftSibling != nullptr)
                destructTree(root->leftSibling);
            if (root->righestChild != nullptr)
                destructTree(root->righestChild);
            
            delete root;
        }

        binHeapItem<T> * findNewMin(void) {
            binHeapItem<T> * tmpMin = nullptr;
            for (unsigned i = 0; i < binomialTrees.size(); i++) {
                binHeapItem<T> * tmp = binomialTrees.next();
                if (i == 0 || tmp->data < tmpMin->data || (tmp->data == tmpMin->data && tmp->timeStamp > tmpMin->timeStamp)) {
                    tmpMin = tmp;
                }
            }
            min = tmpMin;
            return min;
        }

        void mergeTwoLists(CLinkedList<binHeapItem<T>> & list1, CLinkedList<binHeapItem<T>> & list2, CLinkedList<binHeapItem<T>> & destination) {
            size_t i = 0, j = 0;
            while (i < list1.size() && j < list2.size()) {
                if (list1.at(i)->degree < list2.at(j)->degree)
                    destination.insertEnd(list1.at(i++), true);
                else
                    destination.insertEnd(list2.at(j++), true);
            }
 
            while (i < list1.size())
                destination.insertEnd(list1.at(i++), true);
 
            while (j < list2.size())
                destination.insertEnd(list2.at(j++), true);

                
            list1.deleteAll(false);
            treeCount = list1.size();   //TODO maybe delete
                
            list2.deleteAll(false);
            
            // Transfering all to the original list
            for (size_t i = 0; i < destination.size(); i++)
                list1.insertEnd(destination.next(), true);

            connectEqualDegrees();
            treeCount = list1.size();

            destination.deleteAll(false);
            findNewMin();
        }

        void addOtherInTree(CBinomialHeap & tmpHeap, const binHeapItem<T> * tmpItem, const binHeapItem<T> * searchedItem) {
            if (tmpItem->leftSibling != nullptr)
                addOtherInTree(tmpHeap, tmpItem->leftSibling, searchedItem);
            if (tmpItem->righestChild != nullptr)
                addOtherInTree(tmpHeap, tmpItem->righestChild, searchedItem);
            
            if (tmpItem == searchedItem) {
                delete tmpItem;
                return;
            }
            tmpHeap.addItem(tmpItem->data);
            delete tmpItem;
        }
};

int main(void) {
    CBinomialHeap<int> a, b;
    a.rename("a binTrees");
    b.rename("b binTrees");
    binHeapItem<int> * tmp;
    a.addItem(5);
    a.addItem(6);
    // a.addItem(7);
    // a.addItem(8);
    // a.addItem(9);
    // a.addItem(10);
    a.addItem(11);

    
    b.addItem(15);
    b.addItem(16);
    b.addItem(17);
    b.addItem(18);
    tmp = b.addItem(2);
    b.addItem(20);
    b.addItem(21);

    a.merge(b);
    a.printAll();
    std::cout << a.findMin()->data << std::endl;
    a.deleteBinHeapItem(tmp);
    std::cout << a.findMin()->data << std::endl;


    a.printAll();
    return 0;
}