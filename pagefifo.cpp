#include<bits/stdc++.h>

using namespace std;


const int PAGE_TABLE_SIZE = 4;
int pagefaults = 0;
int hits = 0;

vector<int> page_table;
vector<int> page_order;


void reference_page(int page) {
    // If the page is not in the page table
    if (find(page_table.begin(), page_table.end(), page) == page_table.end()) {

        pagefaults++;
        // If the page table is not full, simply add the page to the end
        if (page_table.size() < PAGE_TABLE_SIZE) {
            page_table.push_back(page);
        }


        // If the page table is full, replace the oldest page (i.e., the first one in the page order vector)
        else {
            int oldest_page = page_order.front();
            page_table.erase(remove(page_table.begin(), page_table.end(), oldest_page), page_table.end());
            page_table.push_back(page);
            page_order.erase(page_order.begin());
        }
    }
    else{
        hits++;
    }

    page_order.push_back(page);
}

int main() {

    reference_page(1);
    reference_page(2);
    reference_page(3);
    reference_page(1);
    reference_page(4);
    reference_page(2);
    reference_page(5);

    cout << "Page Table: ";
    for (int i = 0; i < page_table.size(); i++) {
        cout << page_table[i] << " ";
    }
    cout << endl;

    cout << "Total page faults that occour : " << pagefaults << endl;
    cout << "Total Hits occour : " << hits<< endl; 
    return 0;
}
