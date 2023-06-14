#include <bits/stdc++.h>

using namespace std;

const int PAGE_TABLE_SIZE = 4;
int pagefaults = 0;
int hits = 0;

vector<int> page_table;
vector<int> page_order;
unordered_map<int, int> page_time;


void reference_page(int page) {
    // If the page is already in the page table, update its most recent reference time
    if (page_time.find(page) != page_time.end()) {
        page_time[page] = page_order.size();
    }
    // If the page is not in the page table
    else {
        // If the page table is not full, simply add the page to the end
        if (page_table.size() < PAGE_TABLE_SIZE) {
            page_table.push_back(page);
            page_time[page] = page_order.size();
        }
        // If the page table is full, replace the least recently used page (i.e., the one with the smallest reference time)
        else {
            int oldest_page = page_table[0];
            int oldest_time = page_time[oldest_page];
            for (int i = 1; i < page_table.size(); i++) {
                int current_page = page_table[i];
                int current_time = page_time[current_page];
                if (current_time < oldest_time) {
                    oldest_page = current_page;
                    oldest_time = current_time;
                }
            }
            page_table.erase(remove(page_table.begin(), page_table.end(), oldest_page), page_table.end());
            page_time.erase(oldest_page);
            page_table.push_back(page);
            page_time[page] = page_order.size();
        }
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
