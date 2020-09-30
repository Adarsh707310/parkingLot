#include<bits/stdc++.h>
using namespace std;

// enum Operations{Create_parking_lot,Park,Slot_numbers_for_driver_of_age,Leave,Vehicle_registration_number_for_driver_of_age};

string get_command_operation(string command)
{
    string operation="";
    int i=0;
    int len = command.length();

    while(i<len && command[i]!=' ')
    {
        operation+=command[i];
        i++;
    }
    return operation;
}
int get_ParkCapacity(string command)
{
    int capacity=0;
    int i=0;
    int length = command.length();

    while(i<length && command[i]!=' ')
    {
        i++;
    }
    i++;
    while(i<length)
    {
        capacity = capacity*10 + (command[i]-'0');
        i++;
    }
    return capacity;
}

class Person
{
    private:
    string name;
    int age;

    public:
    Person(int age=-1,string name="Jake Johnson")
    {
        this->age = age;
        this->name = name;
    }
    int get_age()
    {
        return age;
    }
    string get_name()
    {
        return name;
    }
};

class Vehicle
{
    protected:
    double mileage;

    public:
    string regNo;
    string VehicleType;

    Vehicle(string regNo="none",string VehicleType="FourWheeler",double mileage=17.5)
    {
        this->regNo = regNo;
        this->VehicleType = VehicleType;
        this->mileage = mileage;
    }
};

class Car: protected Vehicle
{

    public:
    string company_name;
    Person driver;

    Car(Person driver=Person(),string regNo="none",string company_name="Skoda_Octavia")
    {
        this->regNo = regNo;
        this->driver = driver;
        this->company_name = company_name;
    }
    string get_regNo()
    {
        return regNo;
    }
};
class Ticket
{
    private:
    string regNo;
    int slotNo;
    time_t entryTime;

    public:
    Ticket(string regNo,int slotNo):regNo(regNo),slotNo(slotNo)
    {
        this->entryTime =  time(NULL); // cout<<ctime(&my_time);
    }
};

class ParkingLot
{
    private:
    string name;
    string location;
    int capacity;
    priority_queue< int,vector<int>,greater<int> > empty_slots;
    unordered_map<int, set< int >> slots_for_age;
    unordered_map<int, Car> vehicle_at_slot;
    unordered_map<string, int> slot_for_regNo;
    vector<Ticket> all_tickets;

    public:
    ParkingLot(int capacity,string name="Squad_Parking_Lot",string location="Noida_IN")
    {
        this->capacity = capacity;
        for(int i=0;i<capacity;i++)
        {
            empty_slots.push(i);
        }
        vehicle_at_slot.clear();
        slot_for_regNo.clear();
        all_tickets.clear();
    }

    int get_capacity()
    {
        return capacity;
    }

    void update_capacity(int new_capacity)
    {
        capacity = new_capacity;
    }

    int get_slot()
    {
        int best_slot = empty_slots.top();
        empty_slots.pop();
        return best_slot;
    }
    pair<string,int> get_refNo_and_age(string command)
    {
        int len = command.length();
        string operation="";
        int i=0;
        while(i<len && command[i]!=' ')
        {
            operation += command[i];
            i++;
        }
        i++;
        string regNo="";
        while(i<len && command[i]!=' ')
        {
            regNo += command[i];
            i++;
        }
        i++;
        while(i<len && command[i]!=' ')
        {
            i++;
        }
        i++;
        int driver_age=0;
        while(i<len )
        {
            driver_age = driver_age*10 + (command[i]-'0');
            i++;
        }
        pair<string,int> regNo_age = make_pair(regNo,driver_age);

        return regNo_age;
    }
    string handle_park_query(string command)
    {

        pair<string,int> regNo_age = get_refNo_and_age(command);

        string regNo = regNo_age.first;
        int driver_age = regNo_age.second;

        //cout<<regNo<<" "<<driver_age<<endl;

        Person driver = Person(driver_age);

        Car car = Car(driver,regNo);

        // check if we have any empty slot
        if(vehicle_at_slot.size()<capacity)
        {
            int slot_index = get_slot();

            Ticket ticket = Ticket(regNo,slot_index);
            all_tickets.push_back(ticket);

            slots_for_age[driver_age].insert(slot_index);
            vehicle_at_slot[slot_index] = car;
            slot_for_regNo[regNo] = slot_index;

            string message = "Car with vehicle registration number ";
            message += '"';
            message += regNo;
            message += '"';
            message += " has been parked at slot number ";
            message += to_string(slot_index+1);

            return message;
        }
        else
        {
            string message = "No empty slot is available, you can wait or visit other parking lots";
            return message;
        }
    }
    int get_second_intiger_term(string command)
    {
        int len = command.length();
        int i = 0;
        while(i<len && command[i]!=' ')
        {
            i++;
        }
        i++;
        int intiger_value = 0;
        while(i<len)
        {
            intiger_value = intiger_value*10 + (command[i]-'0');
            i++;
        }
        return intiger_value;
    }
    string get_regNo_term(string command)
    {
        int len = command.length();
        int i = 0;
        while(i<len && command[i]!=' ')
        {
            i++;
        }
        i++;
        string regNo = "";
        while(i<len && command[i]!=' ')
        {
            regNo += command[i];
            i++;
        }
        return regNo;
    }
    string handle_Leave_query(string command)
    {
        int leave_slot = get_second_intiger_term(command);

        if(leave_slot>capacity)
        {
            return "slot "+to_string(leave_slot)+" do not exist";
        }
        else if(vehicle_at_slot.find(leave_slot-1)==vehicle_at_slot.end())
        {
            return "slot "+to_string(leave_slot)+" is already empty";
        }
        else
        {
            Car car = vehicle_at_slot[leave_slot-1];
            int driver_age = car.driver.get_age();
            string regNo = car.get_regNo();

            empty_slots.push(leave_slot-1);

            slots_for_age[driver_age].erase(leave_slot);
            vehicle_at_slot.erase(leave_slot);
            slot_for_regNo.erase(regNo);

            string message = "Slot number ";
            message += to_string(leave_slot);
            message += " vacated, the car with vehicle registration number ";
            message += '"';
            message += regNo;
            message += '"';
            message += " left the space, the driver of the car was of age ";
            message += to_string(driver_age);
            return message;
        }
    }
    set<int> handle_Slot_numbers_for_driver_of_age_query(string command)
    {

        int driver_age = get_second_intiger_term(command);
        set<int> set_of_slots = slots_for_age[driver_age];

        return set_of_slots;
    }
    int handle_Slot_number_for_car_with_number_query(string command)
    {
        string regNo = get_regNo_term(command);
        int slotNo = slot_for_regNo[regNo];
        return slotNo;
    }
    vector<string> handle_Vehicle_registration_number_for_driver_of_age_query(string command)
    {
        int driver_age = get_second_intiger_term(command);

        set<int> set_of_slots = slots_for_age[driver_age];

        vector<string> all_regNo_for_age;

        for(auto it: set_of_slots)
        {
           all_regNo_for_age.push_back( vehicle_at_slot[it].get_regNo() );
        }

        return all_regNo_for_age;
    }

};

int main()
{

    ifstream fin;
    string command;
    string operation;
    string message;
    int capacity;
    fin.open("input.txt", ios::in);


    while(true)
    {
        getline(fin,command);
        operation = get_command_operation(command);
        if(operation == "Create_parking_lot")
        {
            capacity = get_ParkCapacity(command);
            break;
        }
    }

    ParkingLot SquadParkingLot = ParkingLot(capacity);
    cout<<"Created parking of "<<capacity<<" slots"<<endl;

    do
    {
        if(!getline(fin,command) || command.empty())
        {
            break;
        }

        operation = get_command_operation(command);

        if(operation=="Park")
        {
            message = SquadParkingLot.handle_park_query(command);
            cout<<message<<endl;
        }
        else if(operation == "Leave")
        {
            message = SquadParkingLot.handle_Leave_query(command);
            cout<<message<<endl;
        }
        else if(operation == "Slot_numbers_for_driver_of_age")
        {
            set<int> all_slots = SquadParkingLot.handle_Slot_numbers_for_driver_of_age_query(command);

            if(all_slots.size()==0)
            {
                cout<<"There is no slot"<<endl;
            }
            else
            {
                cout<<(*all_slots.begin())+1;
                auto it = all_slots.begin();
                it++;
                for(it;it!=all_slots.end();it++)
                {
                    cout<<","<<(*it)+1;
                }
                cout<<endl;
            }
        }
        else if(operation == "Slot_number_for_car_with_number")
        {
            int slotNo = SquadParkingLot.handle_Slot_number_for_car_with_number_query(command);
            cout<<slotNo+1<<endl;
        }
        else if(operation == "Vehicle_registration_number_for_driver_of_age")
        {
            vector<string> all_regNo = SquadParkingLot.handle_Vehicle_registration_number_for_driver_of_age_query(command);

            if(all_regNo.size()==0)
                cout<<endl;
            else
            {
                cout<<all_regNo[0];
                for(int i=1;i<all_regNo.size();i++)
                    cout<<","<<all_regNo[i];
                cout<<endl;
            }
        }
        else
        {
            cout<< "This is a wrong Query kindly check it again"<<endl;
        }
    }
    while(true);
}

