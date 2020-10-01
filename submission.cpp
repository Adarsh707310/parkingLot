#include<bits/stdc++.h>
using namespace std;

// get_command_operation, gives the first word of the line, i.e. operation of the query
// e.g. "Park KA-01-HH-1234 driver_age 21" gives -> "Park"
// e.g. "Leave 2" gives ->  "Leave"
string get_command_operation(string command)
{
    string operation="";
    int i=0;                                             // i is index for iteration
    int len = command.length();                          // len is length of the command

    while(i<len && command[i]!=' ')                      // command[i]!=' '  => iterate until the first space
    {
        operation+=command[i];                          // collecting characters of first word of comm
        i++;
    }
    return operation;
}
// get_second_intiger_term, gives the integer value of second word of the line
// e.g. "Leave 2" gives ->  2
// this function is utilized only where second work is a number
int get_second_intiger_term(string command)
{
    int len = command.length();
    int i = 0;
    while(i<len && command[i]!=' ')
    {
        i++;                                               // we are not interested in the first word
    }
    i++;
    int intiger_value = 0;
    while(i<len && command[i]!=' ')
    {
        intiger_value = intiger_value*10 + (command[i]-'0');   // command[i]-'0' gives the number coresponding to the same character like 6 for '6'
        i++;
    }
    return intiger_value;                            // returning a integer value
}

// Class Definition of Person Class
class Person
{
    private:
    string name;                                    // name is private member variable
    int age;

    public:
    Person(int age=-1,string name="Jake Johnson")   // default values are taken to avoid the hustle of reinitialization
    {
        this->age = age;
        this->name = name;
    }
    int get_age()                                 // function to access the private member outside of class
    {
        return age;
    }
    string get_name()                              // return's the name of the Person
    {
        return name;
    }
};

// Class Definition of Vehicle class
class Vehicle
{
    protected:
    double mileage;                                // kept protected as it is not directly visible in real file vehicle

    public:
    string regNo;                                 // but these are nothing to hide, it is public
    string VehicleType;

    Vehicle(string regNo,string VehicleType="FourWheeler",double mileage=17.5)
    {
                                                // default values are taken to avoid the hustle of reinitialization and to avoid separate non-parameterized constructor
        this->regNo = regNo;
        this->VehicleType = VehicleType;
        this->mileage = mileage;
    }

    virtual double get_mileage()=0;
};
// Class Car inherit the Vehicle class with protected access level
// in order to keep mileage protected in Car class too
class Car: public Vehicle
{

    public:
    string company_name;
    Person driver;



    Car(Person driver=Person(),string regNo="none",string company_name="Skoda_Octavia"):Vehicle(regNo)
    {
        this->driver = driver;
        this->company_name = company_name;
    }
    /*Car(const Car &car2 ) //:Vehicle(car2.regNo)
    {
         this->company_name = car2.company_name;
         this->driver = car2.driver;
         this->mileage = car2.get_mileage();
         this->regNo = car2.get_regNo();
         this->VehicleType = car2.VehicleType;
    }*/
    void operator = (const Car &car2 )
    {
         company_name = car2.company_name;
         driver = car2.driver;
         mileage = car2.mileage;
         regNo = car2.regNo;
         VehicleType = car2.VehicleType;
    }
    void set_regNo(string regNo)
    {
        this->regNo = regNo;
    }
    string get_regNo()                          // protected member can not be access by other all class, this method is needed
    {
        return regNo;
    }
    string get_driver_name()                    // return the name of driver
    {
        return driver.get_name();
    }
    double get_mileage()
    {
        return mileage;
    }
    friend void describe(Car &);                 // describe provides all the information about the Car
};
void describe(Car &car)
{
    cout<<"This is a "<<car.company_name<<" which is "<<car.VehicleType<<" with a mileage of around "<<car.mileage<<
    " Kmph, driver's name is "<<car.driver.get_name()<<", "<<car.driver.get_name()<<
    " is "<<car.driver.get_age()<<endl;
}
// Every Car will be assigned a Entry ticket at the time of entry to the ParkingLot
// class for the same is Defined below
class EntryTicket
{
    private:
    string regNo;                            // member variable for the EntryTicket Class
    int slotNo;
    time_t entryTime;
    time_t exitTime;

    public:
    EntryTicket(string regNo="none",int slotNo=-1):regNo(regNo),slotNo(slotNo)
    { }
    void set_entryTime(time_t entryTime)     // sets the value for emtryTime
    {
        this->entryTime = entryTime;
    }
    void set_exitTime(time_t exitTime)       // sets the value for exitTime
    {
        this->exitTime = exitTime;
    }
    void show_ticket()
    {
        cout<<"Assigned to car no. "<<regNo<<" for the slot "<<slotNo+1<<", Entry Time :"<<ctime(&entryTime)<<", Exit Time :"<<ctime(&exitTime)<<endl;
    }
};

// ParkingLot is riches class of the Parking Lot system
// class definition =>
class ParkingLot
{
    private:
    string name;
    string location;
    int capacity;
    priority_queue< int,vector<int>,greater<int> > empty_slots;  // empty_slots help in determining the empty slot which is nearest to entry gate
    unordered_map<int, set< int >> slots_for_age;                // slots_for_age maintains set of all the slot indexes for a age value
    unordered_map<int, Car> vehicle_at_slot;                     // vehicle_at_slot stores the pair of slot index and object of Car parked at that index
    unordered_map<string, int> slot_for_regNo;                   // slot_for_regNo stores the pair of Car's registration Number and the index of slot at which car is parked
    unordered_map<int,EntryTicket> ticket_of_slot;               // ticket_of_slot stores the pair of slot index and object of EntryTicket assigned to the Car, currently at the slot
    vector<EntryTicket> all_tickets;                             // all_tickets is maintained to keep track of parkin system history


    public:
                                                                // Constructor sets capacity and add all the slots to empty slots priority_queue (this works as min heap here)
    ParkingLot(int capacity,string name="Squad_Parking_Lot",string location="Noida_IN")
    {
        this->capacity = capacity;
        for(int i=0;i<capacity;i++)
        {
            empty_slots.push(i);                               // add all the slots to empty list, as all are empty
        }
        vehicle_at_slot.clear();
        slot_for_regNo.clear();
        all_tickets.clear();                                  // mark rest all empty too
        ticket_of_slot.clear();
    }
    int get_capacity()
    {
        return capacity;
    }

    void update_capacity(int new_capacity)              // we can update capacity in case some slots are not working or new added
    {
        capacity = new_capacity;
    }

    int get_slot()                                       // get_slot() provides base available-slot (nearest to entry gate)
    {
        int best_slot = empty_slots.top();
        empty_slots.pop();
        return best_slot;
    }

    pair<string,int> get_regNo_and_age(string command) // get_regNo_and_age used to find values of age and regNo from Park query line
    {
        int len = command.length();
        int i=0;
        while(i<len && command[i]!=' ')
        {
            i++;                                     // first word is operation
        }
        i++;
        string regNo="";
        while(i<len && command[i]!=' ')
        {
            regNo += command[i];                     //  second word is registration number
            i++;
        }
        i++;
        while(i<len && command[i]!=' ')
        {
            i++;                                     // third is "driver_age "
        }
        i++;
        int driver_age=0;
        while(i<len && command[i]!=' ')
        {
            driver_age = driver_age*10 + (command[i]-'0');  // finally we have numeric values
            i++;
        }
        pair<string,int> regNo_age = make_pair(regNo,driver_age);

        return regNo_age;
    }
    string handle_park_query(string command)                             // Park query is handled here
    {
        pair<string,int> regNo_age = get_regNo_and_age(command);
                                                                      // get_regNo_and_age returns the values separated from command
        string regNo = regNo_age.first;
        int driver_age = regNo_age.second;

        Person driver = Person(driver_age);                           // needed to create Car object

        Car car = Car(driver,regNo);                                    // needed to put in vehicle_at_slot map for this slot_index

        if(slot_for_regNo.find(regNo)!=slot_for_regNo.end())
        {
            string message = "Car "+regNo+" is already present inside SquadParkingLot";
            return message;
        }
        else if(vehicle_at_slot.size()<capacity)                       // if the parking lot have some vacant slots
        {
            if(1>driver_age || driver_age>100)                       // check for query check driver's age
                return "Invalid driver age";

            int slot_index = get_slot();                              // find best slot to assign

            EntryTicket ticket = EntryTicket(regNo,slot_index);
            ticket.set_entryTime(time(NULL));                        // create EntryTicket, with current time

            ticket_of_slot[slot_index] = ticket;                    // pair ticket with slot index

            slots_for_age[driver_age].insert(slot_index);           // same slot index for driver age (we be used later in querying)
            vehicle_at_slot[slot_index] = car;

            slot_for_regNo[regNo] = slot_index;                      // pair the registration number with slot (we be used later in querying)

            string message = "Car with vehicle registration number ";
            message += '"';
            message += regNo;
            message += '"';
            message += " has been parked at slot number ";
            message += to_string(slot_index+1);

            return message;                                            // once changes are done this message need to be printed in a new line
        }
        else                                                           // if the parking lot if fully filled
        {
            string message = "No empty slot is available, you can wait or visit other parking lots";
            return message;
        }
    }
    string handle_Leave_query(string command)                          // Leave query is handled here
    {
        int leave_slot = get_second_intiger_term(command);             // get the leaving slot index

        if(leave_slot>capacity || leave_slot<=0)                                         // check whether this slot index exist
        {
            return "slot "+to_string(leave_slot)+" do not exist";
        }
        else if(vehicle_at_slot.find(leave_slot-1)==vehicle_at_slot.end())  // check whether this slot index already empty
        {
            return "slot "+to_string(leave_slot)+" is already empty";
        }
        else                                                             // if there is a car then steps ->
        {
            Car car = vehicle_at_slot[leave_slot-1];                    // Car object which is parked at the leaving slot
            int driver_age = car.driver.get_age();                     // age of the driver which is with the car parked at leaving slot
            string regNo = car.get_regNo();                            // regNo of Car which is parked at the leaving slot

            EntryTicket ticket = ticket_of_slot[leave_slot-1];
            ticket_of_slot.erase(leave_slot-1);                       // EntryTicket is expired
            ticket.set_exitTime(time(NULL));                          // exitTime is added to the EntryTicket
            all_tickets.push_back(ticket);                            // parking history is maintained by keeping all the tickets

            empty_slots.push(leave_slot-1);                          // the slot is empty now,

            slots_for_age[driver_age].erase(leave_slot-1);       // slot indes need to be deleted from set of driver_age
            vehicle_at_slot.erase(leave_slot-1);                 // there is no vehicle at the slot
            slot_for_regNo.erase(regNo);                       // we do not have slot for this regNo now

            string message = "Slot number ";
            message += to_string(leave_slot);
            message += " vacated, the car with vehicle registration number ";
            message += '"';
            message += regNo;
            message += '"';
            message += " left the space, the driver of the car was of age ";
            message += to_string(driver_age);

            return message;                                 // this message need to be printed as output
        }
    }

    set<int> handle_Slot_numbers_for_driver_of_age_query(string command)   // we want all the slot with given driver age
    {
        int driver_age = get_second_intiger_term(command);     // function get_second_intiger_term returns integer driver age reading from command
        set<int> set_of_slots = slots_for_age[driver_age];     // we maintained the set of slots for all the ages ( is used here )

        return set_of_slots;                                  // the set is returned will be added in output file with appropriate formate
    }

    vector<string> handle_Vehicle_registration_number_for_driver_of_age_query(string command)   // we want all the vehicle regNo for a given driver age ( handle here )
    {
        int driver_age = get_second_intiger_term(command);        // second integer term is the driver age in the command is abstracted out

        set<int> set_of_slots = slots_for_age[driver_age];        // we have set of slot indexes for driver_age from slots_for_age

        vector<string> all_regNo_for_age;                         // let's meke vector of all the regNo here

        for(auto it: set_of_slots)
        {
           if(vehicle_at_slot.find(it)!=vehicle_at_slot.end())
           {
                   all_regNo_for_age.push_back(vehicle_at_slot[it].get_regNo());   // all regNo's are added
           }
       }
       return all_regNo_for_age;                                 // this will be added in the file in the appropriated formate ( code in main() function )
    }
    string get_regNo_term(string command)                         // only used for Slot_number_for_car_with_number query
    {
        int len = command.length();
        int i = 0;
        while(i<len && command[i]!=' ')
        {
            i++;                                                  // first word is "Slot_number_for_car_with_number"
        }
        i++;
        string regNo = "";
        while(i<len && command[i]!=' ')
        {
            regNo += command[i];                                   // this is regNo ( second term here )
            i++;
        }
        return regNo;
    }
    int handle_Slot_number_for_car_with_number_query(string command)   // we want slot_No. for a regNo ( is handle here )
    {
        string regNo = get_regNo_term(command);              // regNo is abstracted from command
        if(slot_for_regNo.find(regNo)==slot_for_regNo.end())
            return -2;
        int slotNo = slot_for_regNo[regNo];                  // slot_for_regNo maps regNo to slotNo in O(1) time complexity
        return slotNo;
    }
    void show_parking_history()                             // For any system keeping track of all the history is very important
    {
        if(all_tickets.size()==0)
            cout<<"No parking history Yet"<<endl;
        else
        {
            for(int i=0;i<all_tickets.size();i++)
            {
                all_tickets[i].show_ticket();              // all the ticket that are expired i.e. car exit completed are described
            }
        }
    }
    void tell_the_status()
    {
        cout<<vehicle_at_slot.size()<<" slots are filled"<<endl;
        for(auto it: vehicle_at_slot)
        {
            cout<<it.first<<" "<<it.second.get_regNo()<<" ";
        }
        cout<<endl<<endl;
    }

};

int main()                                             // main function deals with the creation of system and for reading & writing in files
{

    ifstream fin;                         // required ifstream object to read from txt file
    ofstream fout;                        // required ofstream object to write in the txt file
    string Input_file = "input.txt";      // kindly change this line and put input file name, file should be present in the same folder where the cpp file is
    string Output_file = "output01.txt";  // you will get and .txt file withe this name, containing out puts of the query
    string command;                       // we will read the full line and call it command e.g. "Slot_numbers_for_driver_of_age 21" is single command
    string operation;                     // will tell what we want to do in the following command
    string message;
    int capacity;                         // this will be the capacity of the Parking lot, as in maximum number of slots


    fin.open(Input_file, ios::in);                                // opening the file in "in" mode, to read inputs
    fout.open(Output_file,ios::trunc | ios:: out);                // trunc remover all old content from file

    getline(fin,command);                                          // the first command will be a create command for sure

    capacity = get_second_intiger_term(command);                   // abstract the integer value of parking lot capacity

    ParkingLot SquadParkingLot = ParkingLot(capacity);            //  SquadParkingLot is the primary objective as it encapsulate majority of functions
    fout<<"Created parking of "<<capacity<<" slots"<<endl;        // Output for "Create_parking_lot" query

    cout<<"Welcome to the SquadParkingLot System, refer the output file for query-responses"<<endl;
    // once we have created ParkingLot object, now we can answer other queries
    // based on the current object states
    // we are performing all the queries in O(1) time complexity
    // also space used is not more than O(n) as for each park query, we make sure the capacity constraint is satisfied

    do
    {
        if(!getline(fin,command) || command.empty())            // we need to check if queries are ended.
        {
            break;
        }

        operation = get_command_operation(command);            // operation will decide, how to proceed further

        // As we have ParkingLot object "SquadParkingLot" established,
        // we can now use the specific functions to handle their specific queries

        if(operation=="Park")
        {
            message = SquadParkingLot.handle_park_query(command);   // handle_park_query returns a string, with final output to be printed
            fout<<message<<endl;
        }
        else if(operation == "Leave")
        {
            message = SquadParkingLot.handle_Leave_query(command);  // handle_Leave_query returns a string, with final output to be printed
            fout<<message<<endl;
        }
        else if(operation == "Slot_numbers_for_driver_of_age")     // handle_Slot_numbers_for_driver_of_age_query returns a set of slot indexes for a value of driver age;
        {
            set<int> all_slots = SquadParkingLot.handle_Slot_numbers_for_driver_of_age_query(command);

            if(all_slots.size()==0)                                 // check if there is no slot for given value of driver age
            {
                fout<<endl;
            }
            else                                                    // 1 based indexing is followed here
            {
                fout<<(*all_slots.begin())+1;
                auto it = all_slots.begin();
                it++;
                for(it;it!=all_slots.end();it++)
                {
                    fout<<","<<(*it)+1;                               // indexes are printed as comma separated values
                }
                fout<<endl;
            }
        }
        else if(operation == "Slot_number_for_car_with_number")     // handle_Slot_number_for_car_with_number_query will simply return an integer
        {
            int slotNo = SquadParkingLot.handle_Slot_number_for_car_with_number_query(command);
            if(slotNo==-2)
            {
                string regNo = SquadParkingLot.get_regNo_term(command);
                fout<<"No car with regNo "<<regNo<<" is present in Parking Lot"<<endl;
            }
            else
            {
                fout<<slotNo+1<<endl;                                             // one based indexing is followed
            }

        }
        else if(operation == "Vehicle_registration_number_for_driver_of_age")  // handle_Vehicle_registration_number_for_driver_of_age_query returns vector of all the regNo
                                                                               // for a given value of the driver age
        {
            vector<string> all_regNo = SquadParkingLot.handle_Vehicle_registration_number_for_driver_of_age_query(command);

            if(all_regNo.size()==0)
                fout<<endl;                                            // as givenn in the sample input output nothing will be printed in case of empty vector
            else
            {
                fout<<all_regNo[0];
                for(int i=1;i<all_regNo.size();i++)
                    fout<<","<<all_regNo[i];                           //  we will print comma separated values
                fout<<endl;
            }
        }
        else                                                         // if the operation is not matching with any of the possibility than it can be said as wrong query
        {
            fout<< "This is a wrong Query kindly check it again"<<endl;
        }

    }
    while(true);

    fin.close();
    fout.close();

    return 0;
}
