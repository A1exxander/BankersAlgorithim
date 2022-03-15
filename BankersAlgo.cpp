

#include <iostream>
#include <list>
#include <vector>



class BankersAlgo {
private:
    int resourceA;
    int resourceB; // OS resources
    int procNumber;

    struct process {
        int procID;
        int maxA;
        int maxB;
        int allocatedA;
        int allocatedB;
        int needA;
        int needB; // No need for array of resources, if more than like 4 resources then maybe
    };

    std::list <process> processes{};

public:

    BankersAlgo() : resourceA{ 10 }, resourceB{ 10 }, procNumber{ 0 } {};

    const int& getResourceA() const {

        return resourceA;

    }

    const int& getResourceB() const {

        return resourceB;

    }

    process createProcess() { // Add input val!

        process proc;
        proc.procID = procNumber;

        getInput(proc.maxA, proc.maxB, resourceA, resourceB,  "\nEnter max A & B resources for Process : \n");
        getInput(proc.allocatedA, proc.allocatedB, proc.maxA, proc.maxB, "\nEnter allocated A & B resources for Process : \n");

        proc.needA = proc.maxA - proc.allocatedA;
        proc.needB = proc.maxB - proc.allocatedB;
        
        resourceA -= proc.allocatedA;
        resourceB -= proc.allocatedB;

        ++procNumber;
        return proc;

    }

    void getInput(int& a, int& b, int capA, int capB, const std::string& message) {
    
        std::cout << message;
        std::cin >> a >> b;

        while (a > capA || b > capB) {
        
            std::cout << "\nError. Please re-enter 2 values less than " << capA << " and " << capB;
            std::cin >> a >> b;
        }
    
    }


    void addProcess() {

        process proc{ createProcess() };
        processes.push_back(proc);

    }

    void removeProcess() {
    
        processes.pop_front();
    
    }

    void solveSequence() {

        if (processes.size() == 0) {
        
            std::cout << "\nNo proccesses!\n";

        }
        else {

            std::string safeSequence{ " " };
            bool safeState { true };
            int i {0};
            std::list <int> unsafeStates;  // Stores all unsafe states, so if our queue only contains unsafe states, we can exit and tell user that this queue will lead to deadlock

            std::cout << "\nAVAILIBLE RESOURCES - A : " << resourceA << " , B : " << resourceB;

            while (safeState == true && i < processes.size()) { // No for loop as we may need to perform a context switch and exit when the queue is not done if we enter an unsafe state
            
                
                if (resourceA >= processes.front().needA && resourceB >= processes.front().needB) {

                    std::cout << "\nPROCESS FINISHED : " << processes.front().procID;
                    safeSequence += "PID ", processes.front().procID, " -> ";
                    
                    resourceA += processes.front().allocatedA;
                    resourceB += processes.front().allocatedB;
                    
                    if (std::find(unsafeStates.begin(), unsafeStates.end(), processes.front().procID) != unsafeStates.end() == true) {
                    
                        unsafeStates.remove(processes.front().procID);

                    }

                    removeProcess();
                    ++i;
               }

                else if (checkUnsafeState(unsafeStates)) {
                
                    std::cout << "\nUnsafe state! Cannot continue ";
                    safeState = false;
                
                }

                else {
                
                    if (!contains(unsafeStates, processes.front().procID)){
                        unsafeStates.push_back(processes.front().procID);
                    }
                    processes.push_back(processes.front());
                    removeProcess();

                }
                

            }

            

        }
        
    
    }

    bool contains(const std::list <int>& unsafeStates, int procID) const{
    
        for (auto i{ unsafeStates.begin() }; i != unsafeStates.end(); ++i) {
            if (*i == procID){            
                return true;            
            }
        }
        return false;
    }

    bool checkUnsafeState(const std::list <int>& unsafeStates) const{

        if (unsafeStates.size() != processes.size()) {

            return false;

        }
        else {
        
            for (auto i{ processes.begin() }; i != processes.end(); ++i) {
                
                bool checked{};

                for (auto j{ unsafeStates.begin() }; j != unsafeStates.end(); ++j) {

                    
                
                    if (i->procID == *j) {
                    
                        checked = true;
                    
                    }


                
                }
            
                if (checked == false)
                    return false;
            
            }
        
        }

        return true;

    
    }


};

int main(){
    BankersAlgo b;
    
    b.addProcess();
    b.addProcess();
    b.solveSequence();
}

