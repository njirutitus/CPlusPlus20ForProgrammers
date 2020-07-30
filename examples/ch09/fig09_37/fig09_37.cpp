// fig09_37.cpp
// Serializing and deserializing objects containing private data.
#include <iostream>
#include <fstream>
#include <vector>
#include <fmt/format.h> // In C++20, this will be #include <format> 
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

using namespace std;

class Record {
   // declare serialize as a friend for direct access to private data
   template<class Archive>
   friend void serialize(Archive& archive, Record& record);

public:
   // constructor
   Record(int account = 0, const string& first = "",
      const string& last = "", double balance = 0.0)
      : account{account}, first{first}, last{last}, balance{balance} {}

   // get member functions
   int getAccount() const {return account;}
   string getFirst() const {return first;}
   string getLast() const {return last;}
   double getBalance() const {return balance;}

private:
   int account{};
   string first{};
   string last{};
   double balance{};
};

// function template serialize is responsible for serializing and 
// deserializing Record objects to/from the specified Archive
template <class Archive>
void serialize(Archive& archive, Record& record) {
   archive(cereal::make_nvp("account", record.account),
      cereal::make_nvp("first", record.first),
      cereal::make_nvp("last", record.last),
      cereal::make_nvp("balance", record.balance));
}

// display record at command line
void displayRecords(const vector<Record>& records) {
   for (auto& r : records) {
      cout << fmt::format("{} {} {} {:.2f}\n", r.getAccount(), 
                 r.getFirst(), r.getLast(), r.getBalance());
   }
}

int main() {
   vector<Record> records{
      {100, "Brian", "Blue", 123.45},
      {200, "Sue", "Green", 987.65}
   };

   cout << "Records to serialize:\n";
   displayRecords(records);

   // serialize vector of Records to JSON and store in text file
   if (ofstream output{"records2.json"}; output) {
      cereal::JSONOutputArchive archive{output};
      archive(cereal::make_nvp("records", records)); // serialize records
   }

   // deserialize JSON from text file into vector of Records
   if (ifstream input{"records2.json"}; input) {
      cereal::JSONInputArchive archive{input};
      vector<Record> deserializedRecords{};
      archive(deserializedRecords); // deserialize records
      cout << "\nDeserialized records:\n";
      displayRecords(deserializedRecords);
   }
}


/**************************************************************************
 * (C) Copyright 1992-2021 by Deitel & Associates, Inc. and               *
 * Pearson Education, Inc. All Rights Reserved.                           *
 *                                                                        *
 * DISCLAIMER: The authors and publisher of this book have used their     *
 * best efforts in preparing the book. These efforts include the          *
 * development, research, and testing of the theories and programs        *
 * to determine their effectiveness. The authors and publisher make       *
 * no warranty of any kind, expressed or implied, with regard to these    *
 * programs or to the documentation contained in these books. The authors *
 * and publisher shall not be liable in any event for incidental or       *
 * consequential damages in connection with, or arising out of, the       *
 * furnishing, performance, or use of these programs.                     *
 *************************************************************************/