#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>
#include <ctime>
using namespace std;

string arr[]={"Google","GoogleDrive","Gmail","GoogleMaps","Youtube","Facebook","Instagram","Twitter","Reddit","Upwork","Fiver","LinkedIn","Canva"};
vector<int> popularity;
const int numOfSites = 13;
int HyperLinks[13][13] = {0};

class Time{
	public:
	time_t timeobj;
	string getTime;
	Time(){
		getCurrentTimeString();
	}
	void getCurrentTimeString() {
    	// Current time point
    	auto currentTimePoint = chrono::system_clock::now();

    	// Converting time point to a time_t object
    	time_t currentTime = chrono::system_clock::to_time_t(currentTimePoint);

    	// Converting time_t object to a local time string
    	char timeString[100];
    	strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

    	// Setting values of Data members
    	timeobj=currentTime;
		getTime=timeString;
	}
};
	
struct suggestionNode{
	string site;
	int siteRate;
	suggestionNode* next=NULL;
	public:
	suggestionNode(string s,int p){
		site=s;
		siteRate=p;
	}
};

suggestionNode* suggestionHead=NULL;

void setSuggestionsByPopularity() {
    for (int i = 0; i < 13; i++) {
        suggestionNode* newNode = new suggestionNode(arr[i], popularity[i]);

        if (!suggestionHead) {
            suggestionHead = newNode;
        } else if (suggestionHead->siteRate < newNode->siteRate) {
            newNode->next = suggestionHead;
            suggestionHead = newNode;
        } else {
            suggestionNode* check = suggestionHead;
            while (check->next != NULL && check->next->siteRate > newNode->siteRate) {
                check = check->next;
            }
            newNode->next = check->next;
            check->next = newNode;
        }
    }
}

class BrowsingData{
	BrowsingData * head=NULL;
	BrowsingData * tail=NULL;
	Time* timeVisited= new Time();
	BrowsingData* next=NULL;
	BrowsingData* prev=NULL;

	public:
	BrowsingData * navigator=tail;
	string siteVisited;
	BrowsingData(string site){
		siteVisited=site;
	}
	
	// Method to Visit a new Site
	void visitSite(string name){
		BrowsingData* temp=new BrowsingData(name);
		if(!head){
			head=tail=temp;
			navigator=head;
		}
		else{
			tail->next=temp;
			temp->prev=tail;
			if(navigator==tail){
				navigator=temp;
			}
			tail=temp;
		}
	}

	// Method to remove last site from history
	void removeSite(){
		if(head==NULL){
			cout<<"No browisng history to delete"<<endl;
		}
		else{
			BrowsingData* temp=tail;
			if(tail==head){
				tail=head=navigator=NULL;
				delete(temp);
			}
			else{
				if(navigator==tail)
					navigator=navigator->prev;
				temp=tail;
				tail=tail->prev;
				delete(temp);
			}
		}
	}

	// Method to remove all History
	void removeAllHistory(){
		if(head==NULL){
			cout<<"No browisng history to delete"<<endl;
		}
		else{
			if(tail==head){
				BrowsingData* temp=tail;
				tail=head=navigator=NULL;
				delete(temp);
			}
			else{
				while(head){
					BrowsingData* temp=head;
					head=head->next;
					delete(temp);
				}
				head=tail=navigator=NULL;
			}
			cout<<"Browsing History Cleared!"<<endl;
		}		
	}

	// Method to navigate bacwards in History
	void navigateBackward(int n){
		if(navigator==head){
			cout<<"No previous sites to navigate!"<<endl;
		}
		else{
			int i=0;
			while(i<n ){
				if(navigator==head){
					cout<<"No more browsing data to navigate backward!"<<endl;
					break;
				}
				navigator=navigator->prev;
				cout<<"Navigated to: "<<navigator->siteVisited<<endl;
				i++;
			}
			cout<<"Navigator currently at: "<<navigator->siteVisited<<endl;
			visitSite(navigator->siteVisited);
		}
	}

	// Method to navigate forward in History
	void navigateForward(int n){
		if(navigator==tail){
			cout<<"No next sites to navigate!"<<endl;
		}
		else{
			int i=0;
			while(i<n ){
				if(navigator==tail){
					cout<<"No more browsing data to navigate forward!"<<endl;
					break;
				}
				navigator=navigator->next;
				cout<<"Navigated to: "<<navigator->siteVisited<<endl;
				i++;
			}
			cout<<"Navigator currently at: "<<navigator->siteVisited<<endl;
			visitSite(navigator->siteVisited);
		}
	}

	// Method to delete History by Time
	void deleteHistoryByTime(int minutes) {
        if (head == nullptr) {
            cout << "No browsing history to delete" << endl;
            return;
        }

        auto currentTimePoint = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(currentTimePoint);
        time_t timeThreshold = currentTime - minutes * 60;

        BrowsingData* current = tail;
        while (current && current->timeVisited->timeobj > timeThreshold) {
            cout<<"Deleted: "<<current->siteVisited<<endl;
			BrowsingData* temp = current;
            current = current->prev;
            delete temp;
        }

        tail = current;
        if (tail == nullptr) {
            head = navigator = nullptr;
        } else {
            tail->next = nullptr;
            navigator = tail;
        }

        cout << "Browsing history deleted for the last " << minutes << " minutes." << endl;
    }

	// Method to show hyperlinks attached to a site
	void viewHyperlinks(){
		string current=navigator->siteVisited;
		vector<int> indexes;
		int row=-1;
		for(int i=0;i<(sizeof(arr) / sizeof(arr[0]));i++){
			if(current==arr[i]){
				row=i;
				break;
			}

		} 

		for(int j=0;j<13;j++){
			indexes.push_back(HyperLinks[row][j]);
		}
		
		int j=0;
		cout<<"\nHyperLinks for "<<current<<": "<<endl;
		for(int i=0;i<indexes.size();i++){
			if(indexes[i]==1){
				cout<<(++j)<<". "<<arr[i]<<endl;
			}
		}
	}
	
	// Method to print History
	void printHistory() {
	    BrowsingData* check = tail;
	    if (!check) {
	        cout << "No browsing history to view" << endl;
	        return;
	    }

	    cout << left << setw(20) << "Site Visited" << setw(20) << "Time Visited" << endl;
	    while (check) {
	        cout << left << setw(20) << check->siteVisited << setw(20) << check->timeVisited->getTime << endl;
	        check = check->prev;
	    }
	}

};


class Edge {
	public:
    int src, dest;
    int weight = 0;
};

class Graph {
	public:
	vector<Edge> edges;
	int numVertices;

	Graph(int numV) : numVertices(numV) {}

	void addEdge(int src, int dest, int weight) {
	    Edge edge;
	    edge.src = src;
	    edge.dest = dest;
	    edge.weight = weight;
	    edges.push_back(edge);
	}
};

// Method to check the index of a site in array
int getIndex(string s){
	for(int i=0;i<13;i++){
		if(arr[i]==s){
			return i;
		}
	}
	return 0;
}

// Method to set Popularity of Sites from File
void setPopularityFromFile(){
      ifstream f("Analytic.csv");
   if (!f.is_open()) {
       cout << "Error opening file!" << endl;
       return;
   } 
   string line1, word1;
   vector<string> lineData1;
   while (getline(f, line1)) {
	   stringstream s(line1);
       while (getline(s, word1, ',')) {
           lineData1.push_back(word1);
       }
       stringstream s2(lineData1[1]);
       int sitePopularity;
       s2 >> sitePopularity;
	   if(sitePopularity!=0){
		popularity.push_back(sitePopularity);
	   }
       lineData1.clear();
   }
   f.close();  
}

// Method to make graph for hyperlinks of website
void setGraphFromFile(){
	ifstream fin("Dataset.csv");
   	if (!fin.is_open()) {
       cerr << "Error opening file!" << endl;
       return;
   	}
   string line, word;
   vector<string> lineData;
   int lines = 0;
   while (getline(fin, line)) { 
       stringstream s(line);
       while (getline(s, word, ',')) {
           lineData.push_back(word);
       }
       stringstream s2(lineData[0]);
       string originWebsiteName;
       s2 >> originWebsiteName;
       stringstream s3(lineData[1]);
       string destinationWebsiteName;
       s3 >> destinationWebsiteName;
       if (lines != 0) {
           HyperLinks[getIndex(originWebsiteName)][getIndex(destinationWebsiteName)] = 1;
       }
       lineData.clear();
       lines++;
   }
   fin.close();
}

// Method to Print the graph's matrix
void printMatrix(){
    cout << "HyperLink Matrix:" << endl;
    cout << setw(14) << " ";
    for (int i = 0; i < numOfSites; i++) {
        cout << setw(14) << arr[i];
    }
    cout << endl;

    for (int i = 0; i < numOfSites; i++) {
        cout << setw(13) << arr[i];
        for (int j = 0; j < numOfSites; j++) {
            cout << setw(13) << fixed << setprecision(2) << HyperLinks[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {

	BrowsingData* b= new BrowsingData("Starting Browser");
	setGraphFromFile();
	setPopularityFromFile();
	setSuggestionsByPopularity();
	printMatrix();
	b->visitSite("Google");
	cout<<".........................................................."<<endl;
	cout<<"               Welcome to Browsing System!"<<endl;
	int choice;
	while(true){
		if(!b->navigator){
			cout<<"You have cleared all the browsing data. Run program again to continue!"<<endl;
			break;
		}
		cout<<".........................................................."<<endl;
		cout<<"\nYou are currrently at site: "<< b->navigator->siteVisited<<endl;;


		cout<<"\nChoose one of the following: "<<endl;
		cout<<"1. View Sites by Popularity"<<endl;
		cout<<"2. View site Hyperlinks"<<endl;
		cout<<"3. View Browsing History"<<endl;
		cout<<"4. Navigate Backward from history"<<endl;
		cout<<"5. Navigate Forward from history"<<endl;
		cout<<"6. Remove last site from history"<<endl;
		cout<<"7. Remove history by Time"<<endl;
		cout<<"8. Remove all history"<<endl;
		cout<<"9. Visit new site"<<endl;
		cout<<"10. Exit"<<endl;
		cout<<"Choosen Option: ";

		cin>>choice;

		switch(choice){
			case 1:{
				suggestionNode* check=suggestionHead;
				cout<<"Viewing Sites by popularity: "<<endl;
				while(check){
					cout<<check->site<<endl;
					check=check->next;
				}
				break;
			}
			case 2:{
				b->viewHyperlinks();
				break;
			}
			case 3:{
				cout<<"\nViewing Browsing History: "<<endl;
				b->printHistory();
				break;
			}
			case 4:{
				int steps;
				cout<<"Enter the number of steps that you want to navigate backwards: ";
				cin>>steps;
				b->navigateBackward(steps);
				break;
			}
			case 5:{
				int steps;
				cout<<"Enter the number of steps that you want to navigate forward: ";
				cin>>steps;
				b->navigateForward(steps);
				break;
			}
			case 6:{
				b->removeSite();
				break;
			}
			case 7:{
				int mins;
				cout<<"Enter the number of miuntes for which you want to delete the browsing history for: ";
				cin>>mins;
				b->deleteHistoryByTime(mins);
				break;
			}
			case 8:{
				b->removeAllHistory();
				break;
			}
			case 9:{
				int choice1;
				cout<<"Choose the site that you want to visit: "<<endl;
				for(int i=0;i<13;i++){
					cout<<(i+1)<<". "<<arr[i]<<endl;
				}
				cout<<"Choosen Option: ";
				cin>>choice1;
				switch(choice1){
					case 1:{
						b->visitSite("Google");
						break;
					}
					case 2:{
						b->visitSite("GoogleDrive");
						break;
					}
					case 3:{
						b->visitSite("Gmail");
						break;
					}
					case 4:{
						b->visitSite("GoogleMaps");
						break;
					}
					case 5:{
						b->visitSite("Youtube");
						break;
					}
					case 6:{
						b->visitSite("Facebook");
						break;
					}
					case 7:{
						b->visitSite("Instagram");
						break;
					}
					case 8:{
						b->visitSite("Twitter");
						break;
					}
					case 9:{
						b->visitSite("Reddit");
						break;
					}
					case 10:{
						b->visitSite("Upwork");
						break;
					}
					case 11:{
						b->visitSite("Fiver");
						break;
					}
					case 12:{
						b->visitSite("LinkedIn");
						break;
					}
					case 13:{
						b->visitSite("Canva");
						break;
					}
					default:{
						cout<<"Invalid Input!"<<endl;
						break;
					}
				}
				break;
			}
			case 10:{
				cout<<"Program Execution Complete!"<<endl;
				cout<<"Thanks for Visiting!"<<endl;
				break;
			}
			default:{
				cout<<"Invalid Input!"<<endl;
				break;
			}
		}
		if(choice==10){
			break;
		}
	}
   return 0;
}

