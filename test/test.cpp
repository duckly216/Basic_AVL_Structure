#include <catch2/catch_test_macros.hpp>
#include <iostream>
// uncomment and replace the following with your own headers
#include <charconv>

#include "AVL.h"

using namespace std;
string extractName(stringstream& in)
{
	if (in.peek() != '\"')
		return "";
	string name;
	getline(in, name,'\"');
	getline(in, name,'\"');
	regex validWord = regex("^[a-zA-Z0-9_]{1,1000}$");
	if (!regex_match(name, validWord))
		return "";
	return name;
}
// Only verifies if the input is alphanumerical,
// insert method ensures it adheres to specifications
string verifyNextWord(stringstream& in)
{
	string word;
	if(!(in >> word))
		word = "";
	// Verifies if it has a-z,A-Z. 0-9, and if it is length 10000
	regex validWord = regex("^[a-zA-Z0-9_]{1,1000}$");
	if (!regex_match(word, validWord))
		return "";
	return word;
}
// you must write 5 unique, meaningful tests for credit on the testing portion of this project!


// Jorge Ramirez : ID - 49259007
TEST_CASE("Incorrect Input Test", "[Input-Test]"){
	AVL myAvl;
	bool testSuccess;
	SECTION("Name With Numbers") {
		testSuccess = myAvl.insert("James67Baxter", "00007823");
		REQUIRE(testSuccess == false);
	};
	SECTION("Name With Special Characters")
	{
		testSuccess = myAvl.insert("Hello World!", "00882139");
		REQUIRE(testSuccess == false);
	}
	SECTION("No Name and Valid ID")
	{
		testSuccess = myAvl.insert("", "00007823");
		REQUIRE(testSuccess == false);
	}
	SECTION("No ID and Valid Name")
	{
		testSuccess = myAvl.insert("James67Baxter", "");
		REQUIRE(testSuccess == false);
	}
	SECTION("Incorrect ID Length")
	{
		testSuccess = myAvl.insert("Diana Helen", "5462");
		REQUIRE(testSuccess == false);
	}
	SECTION("Incorrect ID content")
	{
		testSuccess = myAvl.insert("Jack Ohph", "rter5462");
		REQUIRE(testSuccess == false);
	}
	SECTION("Same # ID")
	{
		testSuccess = myAvl.insert("Jack Ohph", "22222222");
		REQUIRE(testSuccess == true);
	}
}
TEST_CASE("Edge Cases", "[Edge Cases]")
{

	SECTION("Removing Non-Existent Node")
	{
		AVL myAvl;
		SECTION("Trying to Delete Name")
		{
			myAvl.remove("Eris Morn");
			REQUIRE(myAvl.getRecentOutput() == "unsuccessful\n");
		}
		SECTION("Trying to Delete ID not in Tree")
		{
			myAvl.remove("00000000");
			REQUIRE(myAvl.getRecentOutput() == "unsuccessful\n");
		}

	}
	SECTION("Removing in an Empty Tree")
	{
		AVL myAvl;
		myAvl.remove("00000000");
		REQUIRE(myAvl.getRecentOutput() == "unsuccessful\n");
	}
	SECTION("Removing an Invalid ID")
	{
		AVL myAvl;

		SECTION("Incorrect ID Length")
		{
			myAvl.remove("000000");
			REQUIRE(myAvl.getRecentOutput() == "unsuccessful\n");
		}
		SECTION("Incorrect ID content")
		{
			myAvl.remove("abcdefgh");
			REQUIRE(myAvl.getRecentOutput() == "unsuccessful\n");
		}
	}
	SECTION("Inserting Duplicate IDs")
	{
		AVL myAvl;
		std::vector<Student> expectedOutput, actualOutput;
		myAvl.insert("James Baxter", "00007823");
		REQUIRE(myAvl.getRecentOutput() == "successful\n");
		myAvl.insert("Jeremy Hollow", "00007823"); //Duplicate ID
		REQUIRE(myAvl.getRecentOutput() == "unsuccessful\n");

		myAvl.insert("Kyle Ren", "00005623");
		REQUIRE(myAvl.getRecentOutput() == "successful\n");
		myAvl.insert("Ren Kyle", "00005623"); //Duplicate ID
		REQUIRE(myAvl.getRecentOutput() == "unsuccessful\n");


		expectedOutput = { Student("James Baxter", "00007823"), Student("Kyle Ren", "00005623")};
		actualOutput = myAvl.inorder();

		REQUIRE(expectedOutput.size() == actualOutput.size());
		sort(expectedOutput.begin(), expectedOutput.end());
		REQUIRE(expectedOutput == actualOutput);

	}
	SECTION("Inserting Duplicate Names")
	{
		AVL myAvl;
		std::vector<Student> expectedOutput, actualOutput;
		myAvl.insert("James Baxter", "00007823");
		REQUIRE(myAvl.getRecentOutput() == "successful\n");
		myAvl.insert("James Baxter", "00407823"); //Duplicate Name
		REQUIRE(myAvl.getRecentOutput() == "successful\n");

		myAvl.insert("Kyle Ren", "00005623");
		REQUIRE(myAvl.getRecentOutput() == "successful\n");
		myAvl.insert("Kyle Ren", "03400562"); //Duplicate Name
		REQUIRE(myAvl.getRecentOutput() == "successful\n");


		expectedOutput = {
			Student("James Baxter", "00007823"), Student("Kyle Ren", "00005623"),
			Student("James Baxter", "00407823"), Student("Kyle Ren", "03400562")
		};
		actualOutput = myAvl.inorder();

		REQUIRE(expectedOutput.size() == actualOutput.size());
		sort(expectedOutput.begin(), expectedOutput.end());
		REQUIRE(expectedOutput == actualOutput);
	}
	SECTION("Inserting Empty Node")
	{
		AVL myAvl;
		myAvl.insert("", "");
		REQUIRE(myAvl.getRecentOutput() == "unsuccessful\n");
		myAvl.insert("", "00007823");
		REQUIRE(myAvl.getRecentOutput() == "unsuccessful\n");
	}
	SECTION("Back-to-Back Rotations")
	{
		AVL myAvl;
		myAvl.insert("James Baxter", "00000001");
		myAvl.insert("Kyle Ren", "00000002");
		myAvl.insert("Carmen Santiago", "00000003");
		myAvl.insert("Jorge Ramirez", "00000004");
		myAvl.insert("Ikora Rey", "00000005");
		SECTION("Balance Check")
			REQUIRE(myAvl.checkIfBalanced(myAvl.getRoot()) == true);
		SECTION("Root Check")
			REQUIRE(myAvl.getRoot()->getGatorID() == "00000002");
		SECTION("Child Check")
		{
			REQUIRE(myAvl.getRoot()->left->getGatorID() == "00000001");
			REQUIRE(myAvl.getRoot()->right->getGatorID() == "00000004");
		}
	}
	SECTION("Searching for a blank input")
	{
		AVL myAvl;
		myAvl.insert("James Baxter", "00000001");
		myAvl.search("");
		REQUIRE(myAvl.getRecentOutput() == "unsuccessful\n");
	}
}
TEST_CASE("Deletion Cases", "[Commands][Deletion]")
{
	SECTION("No Children")
	{
		AVL myAvl;
		myAvl.insert("James Baxter", "00000001");
		myAvl.insert("Kyle Ren", "00000002");
		myAvl.remove("00000002");
		REQUIRE(myAvl.getRecentOutput() == "successful\n");
		REQUIRE(myAvl.getRoot()->left == nullptr);
	}
	SECTION("One Child")
	{
		AVL myAvl;
		SECTION("One Left Child")
		{
			myAvl.insert("James Baxter", "00000005");
			myAvl.insert("Kyle Ren", "00000003");
			myAvl.insert("Carmen Santiago", "00000001");
			myAvl.remove("00000003");
			REQUIRE(myAvl.getRecentOutput() == "successful\n");
			REQUIRE(myAvl.getRoot()->left->getGatorID() == "00000001");
		}
		SECTION("One Right Child")
		{
			myAvl.insert("James Baxter", "00000005");
			myAvl.insert("Kyle Ren", "00000003");
			myAvl.insert("Carmen Santiago", "00000004");
			myAvl.remove("00000003");
			REQUIRE(myAvl.getRecentOutput() == "successful\n");
			REQUIRE(myAvl.getRoot()->getGatorID() == "00000004");
		}

	}
	SECTION("Two Children")
	{
		AVL myAvl;
		myAvl.insert("Daft Punk", "00000008");
		myAvl.insert("James Baxter", "000000010");
		myAvl.insert("Kyle Ren", "00000003");
		myAvl.insert("Carmen Santiago", "00000001");
		myAvl.insert("Jorge Ramirez", "00000004");

		myAvl.remove("00000003");
		REQUIRE(myAvl.getRecentOutput() == "successful\n");
		REQUIRE(myAvl.getRoot()->getGatorID() == "00000004");
	}
	SECTION("Two Children (Successor Check)")
	{
		AVL myAvl;
		myAvl.insert("A", "00000010");
		myAvl.insert("B", "00000015");
		myAvl.insert("C", "00000005");
		myAvl.insert("D", "00000002");
		myAvl.insert("E", "00000013");
		myAvl.insert("F", "00000017");
		myAvl.insert("G", "00000011");
		myAvl.insert("H", "00000012");
		myAvl.insert("I", "00000020");
		myAvl.remove("00000010");

		REQUIRE(myAvl.getRoot()->getGatorID() == "00000011");

	}
}
TEST_CASE("Insert Command and Rotation Cases Test", "[Commands][Insert][Rotations]")
{
	// Using Same Logic as in Main Method, except using stringstream type instead of istringstream
	SECTION("Insert Command")
	{
		AVL tree;
		string name, ID, command;
		stringstream cmdLine;
		SECTION("One Quotation Mark")
		{
			cmdLine << "insert \"Mark Twain 12345678";
			// First Token should be instruction
			getline(cmdLine,command, ' ');
			if (command == "insert")
			{
				string name, ID;
				name = extractName(cmdLine);
				ID = verifyNextWord(cmdLine);
				if (name == "" || ID == "")
					// Results in unsuccessful if it doesnt have either a name or ID
						tree.insert("", "");
				else
					tree.insert(name, ID);
			}
			REQUIRE(tree.getRecentOutput() == "unsuccessful\n");
		}
		SECTION("Quotation Mark Somewhere other than Beginning")
		{
			cmdLine << "insert Mark\" Twain 12345678";
			// First Token should be instruction
			getline(cmdLine,command, ' ');
			if (command == "insert")
			{
				string name, ID;
				name = extractName(cmdLine);
				ID = verifyNextWord(cmdLine);
				if (name == "" || ID == "")
					// Results in unsuccessful if it doesnt have either a name or ID
						tree.insert("", "");
				else
					tree.insert(name, ID);
			}
			REQUIRE(tree.getRecentOutput() == "unsuccessful\n");
		}
		SECTION("Quotation Mark Before End of Name")
		{
			cmdLine << "insert \"Mark Twa\"in 12345678";
			// First Token should be instruction
			getline(cmdLine,command, ' ');
			if (command == "insert")
			{
				string name, ID;
				name = extractName(cmdLine);
				ID = verifyNextWord(cmdLine);
				if (name == "" || ID == "")
					// Results in unsuccessful if it doesnt have either a name or ID
						tree.insert("", "");
				else
					tree.insert(name, ID);
			}
			REQUIRE(tree.getRecentOutput() == "unsuccessful\n");
		}
		SECTION("Quotation Mark On Whole Command")
		{
			cmdLine << "\"insert \"Mark Twain 12345678\"";
			// First Token should be instruction
			getline(cmdLine,command, ' ');
			if (command == "insert")
			{
				string name, ID;
				name = extractName(cmdLine);
				ID = verifyNextWord(cmdLine);
				if (name == "" || ID == "")
					// Results in unsuccessful if it doesnt have either a name or ID
						tree.insert("", "");
				else
					tree.insert(name, ID);
			}
			else
				tree.insert("", ""); // Unsuccessful attempt at insert
			REQUIRE(tree.getRecentOutput() == "unsuccessful\n");
		}

	}
	SECTION("Single Node Insertion")
	{
		AVL myAvl;
		myAvl.insert("James Baxter", "00007823");
		SECTION("Balance Check")
			REQUIRE(myAvl.checkIfBalanced(myAvl.getRoot()) == true);
		SECTION("Root Check")
			REQUIRE(myAvl.getRoot()->getGatorID() == "00007823");
	}
	SECTION("Empty Tree/ Pre - Insertion")
	{
		AVL myAvl;
		SECTION("Balance Check")
		{
			REQUIRE(myAvl.checkIfBalanced(myAvl.getRoot()) == true);
		}
	}
	SECTION("Left Left case")
	{
		AVL myAvl1;
		myAvl1.insert("Redrix Shaxx", "00000008");
		myAvl1.insert("Hello World", "00000004");
		myAvl1.insert("Diana Helen", "00000002");

		SECTION("Balance Check")
			REQUIRE(myAvl1.checkIfBalanced(myAvl1.getRoot()) == true);
		SECTION("Root Check")
			REQUIRE(myAvl1.getRoot()->getGatorID() == "00000004");
		SECTION("Child Check"){
			REQUIRE(myAvl1.getRoot()->left->getGatorID() == "00000002");
			REQUIRE(myAvl1.getRoot()->right->getGatorID() == "00000008");
		}
	}
	SECTION("Right Right case")
	{
		AVL myAvl2;
		myAvl2.insert("RAT", "00000002");
		myAvl2.insert("Hello World", "00000004");
		myAvl2.insert("Diana Helen", "00000008");
		SECTION("Balance Check")
			REQUIRE(myAvl2.checkIfBalanced(myAvl2.getRoot()) == true);
		SECTION("Root Check")
			REQUIRE(myAvl2.getRoot()->getGatorID() == "00000004");
		SECTION("Child Check"){
			REQUIRE(myAvl2.getRoot()->left->getGatorID() == "00000002");
			REQUIRE(myAvl2.getRoot()->right->getGatorID() == "00000008");
		}
	}
	SECTION("Right Left case ")
	{
		AVL myAvl3;
		myAvl3.insert("RAT", "00000008");
		myAvl3.insert("Hello World", "00000019");
		myAvl3.insert("Diana Helen", "00000010");
		INFO("Root was: " << myAvl3.getRoot()->getGatorID());
		SECTION("Balance Check ")
			REQUIRE(myAvl3.checkIfBalanced(myAvl3.getRoot()) == true);
		SECTION("Root Check")
			REQUIRE(myAvl3.getRoot()->getGatorID() == "00000010");
		SECTION("Child Check")
		{
			REQUIRE(myAvl3.getRoot()->left->getGatorID() == "00000008");
			REQUIRE(myAvl3.getRoot()->right->getGatorID() == "00000019");
		}
	}

	SECTION("Left Right case")
	{
		AVL myAvl4;
		myAvl4.insert("RAT", "00000008");
		myAvl4.insert("Hello World", "00000002");
		myAvl4.insert("Diana Helen", "00000004");
		SECTION("Balance Check")
			REQUIRE(myAvl4.checkIfBalanced(myAvl4.getRoot()) == true);
		SECTION("Root Check")
			REQUIRE(myAvl4.getRoot()->getGatorID() == "00000004");
		SECTION("Child Check")
		{
			REQUIRE(myAvl4.getRoot()->left->getGatorID() == "00000002");
			REQUIRE(myAvl4.getRoot()->right->getGatorID() == "00000008");
		}

	}

}
TEST_CASE("Rotation Cases Test in Non-Root Nodes", "[Commands][Insert][Rotations]")
{
	SECTION("Left Left case")
	{
		AVL myAvl1;
		myAvl1.insert("Redrix Shaxx", "00000008");
		myAvl1.insert("Hello World", "00000004");
		myAvl1.insert("Juse", "00000009");
		myAvl1.insert("Diana Helen", "00000002");
		myAvl1.insert("James Baxter", "00000001");

		SECTION("Balance Check")
			REQUIRE(myAvl1.checkIfBalanced(myAvl1.getRoot()) == true);
		SECTION("Root Check")
			REQUIRE(myAvl1.getRoot()->getGatorID() == "00000008");
		SECTION("Child Check")
		{
			REQUIRE(myAvl1.getRoot()->left->getGatorID() == "00000002");
			REQUIRE(myAvl1.getRoot()->right->getGatorID() == "00000009");
		}
	}
	SECTION("Right Right case")
	{
		AVL myAvl2;
		myAvl2.insert("Himmel the Hero", "00000010");
		myAvl2.insert("Hello World", "00000020");
		myAvl2.insert("Diana Helen", "00000030");
		myAvl2.insert("Juse Baxter", "00000005");
		myAvl2.insert("Diana Helen", "00000040");
		SECTION("Balance Check")
			REQUIRE(myAvl2.checkIfBalanced(myAvl2.getRoot()) == true);
		SECTION("Root Check")
			REQUIRE(myAvl2.getRoot()->getGatorID() == "00000020");
		SECTION("Child Check"){
			REQUIRE(myAvl2.getRoot()->left->getGatorID() == "00000010");
			REQUIRE(myAvl2.getRoot()->right->getGatorID() == "00000030");
		}
	}
	SECTION("Right Left case")
	{
		AVL myAvl3;
		myAvl3.insert("RAT", "00000012");
		myAvl3.insert("Hello World", "00000019");
		myAvl3.insert("Diana Helen", "00000005");
		myAvl3.insert("Juse Baxter", "00000021");
		myAvl3.insert("Juse Baxter", "00000020");
		INFO("Root was: " << myAvl3.getRoot()->getGatorID());
		SECTION("Balance Check ")
			REQUIRE(myAvl3.checkIfBalanced(myAvl3.getRoot()) == true);
		SECTION("Root Check")
			REQUIRE(myAvl3.getRoot()->getGatorID() == "00000012");
		SECTION("Child Check")
		{
			REQUIRE(myAvl3.getRoot()->left->getGatorID() == "00000005");
			REQUIRE(myAvl3.getRoot()->right->getGatorID() == "00000020");
		}
	}
	SECTION("Left Right case")
	{
		AVL myAvl4;
		myAvl4.insert("Redrix Shaxx", "00000008");
		myAvl4.insert("Hello World", "00000004");
		myAvl4.insert("Juse", "00000009");
		myAvl4.insert("Diana Helen", "00000001");
		myAvl4.insert("James Baxter", "00000002");

		SECTION("Balance Check")
			REQUIRE(myAvl4.checkIfBalanced(myAvl4.getRoot()) == true);
		SECTION("Root Check")
			REQUIRE(myAvl4.getRoot()->getGatorID() == "00000008");
		SECTION("Child Check")
		{
			REQUIRE(myAvl4.getRoot()->left->getGatorID() == "00000002");
			REQUIRE(myAvl4.getRoot()->right->getGatorID() == "00000009");
		}
	}
}
TEST_CASE("100 Node Insertion and Random 10 Node Deletion", "[Commands][Insert][Deletion]"){
	AVL inputTree;
	vector<Student> expectedOutput, actualOutput;
	SECTION("100 Node Insertion")
	{
		for(int i = 0; i < 100; )
		{
			int randomID = rand() % 99999999 + 10000000;
			char randLetter = char(65 + rand() % 26);
			string letter ;
			letter = randLetter;

			Student sampleStu(letter,to_string(randomID));
			// Adds the Student objects in-order

			bool inserted = inputTree.insert(letter,to_string(randomID));
			if(inserted){
				i++;
				expectedOutput.push_back(sampleStu);
			}
		}
		actualOutput = inputTree.inorder();
		REQUIRE(expectedOutput.size() == actualOutput.size());
		sort(expectedOutput.begin(), expectedOutput.end());
		REQUIRE(expectedOutput == actualOutput);
		SECTION("10 Random Node Deletion")
		{
			for(int i = 0; i < 10;)
			{
				int randomInput = rand() % expectedOutput.size();
				bool removed = inputTree.removeInorder(randomInput);
				if (removed)
				{
					expectedOutput.erase(expectedOutput.begin() + randomInput);
					i++;
				}

			}
			actualOutput = inputTree.inorder();
			REQUIRE(expectedOutput.size() == actualOutput.size());
			REQUIRE(expectedOutput == actualOutput);

		}
	}
}
