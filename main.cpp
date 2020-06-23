#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>

using namespace std;

void Quicksort(int* arr, int low, int high);
int partitioning(int arr[], int low, int high);
void swap(int* x, int* y);

class LinearCurves{

    struct Line{
        double a;
        double b;
        string firm;
    };

    private:

        Line* lines;
        int numOfLines;
        string title;
        bool slope;
        string concern;
        int per;
        int ext;

    public:
        LinearCurves()
        {
            LinearCurves(false, "", 0, 0);
        }

        LinearCurves(bool slope)
        {
            LinearCurves(slope, "", 0, 0);
        }

        LinearCurves(bool newSlope, string newTitle)
        {
            LinearCurves(newSlope, newTitle, 0, 0);
        }

        LinearCurves(bool newSlope, string newTitle, int period, int newExt)
        {
            per = period;
            slope = newSlope;
            ext = newExt;
            if(slope)
                newExt *= (-1);

            if(slope)
                concern = "supplyproducercost";
            else
                concern = "demandconsumerbenefit";

            title = newTitle;

            cout << "Enter the variables for equations of individual " << concern.substr(0,6) << " curve." << endl;
            cout << "Equation format: p=" << (slope ? "c+d" : "a-b") << "q" << endl;

            cout << endl << "How many individual " << concern.substr(0,6) << " curves are there? "; cin >> numOfLines; cin.clear(); cin.ignore('256', '\n');
            cout << endl;
            lines = new Line[numOfLines];

            for(int count = 0; count < numOfLines; count++)
            {
                cout << "Individual " << concern.substr(0,6) << " curve#" << count+1 << " -- " << endl;
                cout << "Enter the firm's name: "; getline(cin, lines[count].firm);
                cout << "Enter a: "; cin >> lines[count].a; cin.ignore('256', '\n');
                cout << "Enter b: "; cin >> lines[count].b; cin.ignore('256', '\n');
                cout << endl;
            }
            //Testing
            MergeSort(lines, 0, numOfLines-1);
            printEqns();
            calculate();
        }

        void calculate()
        {
            cout << "Calculate: (1) Marginal " << concern.substr(14,concern.size() - 14);
            cout << " (2) Marginal social " << concern.substr(14,concern.size() - 14);
            cout << " (3) Individual " << concern.substr(6,8) << " surplus";
            cout << " (4) Market " << concern.substr(6,8) << " surplus";
            cout << " (5) Total " << concern.substr(6,8) << " surplus" << endl;
            cout << "Enter your choice: "; int choice; cin >> choice;
            cout << endl;

            int index, quantity = -1;
            cin.ignore('256', '\n');
            if(choice%2)
            {
                cout << endl << "Enter firm's name: "; string firm; getline(cin, firm);

                for(int count = 0; count < numOfLines; count++)
                {
                    if(lines[count].firm == firm)
                        index = count;
                }
                if(index == -1)
                    cout << "There is no information about this company. " << endl;
            }

            cout << "Enter quantity: "; cin >> quantity;
            cout << endl;

            double pr, marketPrice;

            switch(choice)
            {
                case 1:
                    cout << "Marginal " << concern.substr(14,concern.size() - 14) << " of " << lines[index].firm << "'s individual " << concern.substr(0,6) << " curve: " << getPrice(lines[index], quantity) << endl;
                    break;

                case 2:
                    cout << "Marginal social " << concern.substr(14,concern.size() - 14) << " of the market for " << title << ": " << calculateMarginalSocial(quantity) << endl;
                    break;

                case 3:
                    cout << "Individual " << concern.substr(6,8) << " surplus for " << lines[index].firm << ": " << calculateIndividualSurplus(lines[index], getPrice(lines[index], quantity),  quantity) << endl;
                    break;

                case 4:
                    //marketPrice = calculateMarginalSocial(quantity);
                    cout << concern.substr(6, 8) << " surplus for the " << title << " market: " << calculateMarketSurplus(getPrice(lines[numOfLines/2], quantity/numOfLines), quantity/numOfLines, numOfLines/2) << endl;
                    break;

                case 5:
                    cout << "Total " << concern.substr(14,concern.size() - 14) << " surplus for the " << title << " market: " << calculateTotalSocial(quantity) << endl;
                    break;

                default:
                    cout << "Sorry! Your choice of input is invalid. " << endl;
            }
        }

        double getPrice(Line line, int qty)
        {
            if(slope)
                qty*=(-1);

            return (line.a-line.b*qty);
        }

        double getQuantity(Line line, double price)
        {
            if(!slope)
                line.b*=(-1);

            return (price-line.a)/line.b;
        }

        double correspondent(Line line, bool var, double pq)       //p = a + bq,    p = a - bq;     q = (p - a)/b,  q = (a - p)/b;
        {
            //price
            if(var)
            {
                if(slope)
                    pq*=(-1);

                return (line.a-line.b*pq);
            }
            //quantity
            if(!slope)
                line.b*=(-1);

            return (pq-line.a)/line.b;
        }

        double calculateMarginalSocial(int quantity)
        {
            if(quantity <= (per * numOfLines) )
                return lines[numOfLines-(quantity/per+1)].a;

            return getPrice(lines[numOfLines/2], quantity/numOfLines);
        }

        double calculateIndividualSurplus(Line line, double price, int quantity)
        {
            if( ( slope && (price > ext) ) || ( !slope && (price < ext) ) )
                return 0.0;

            return pow(quantity, 2) / 2.0 * line.b;
        }

        double calculateMarketSurplus(double price, int qty, int index)
        {
            return (slope?1:-1)*(price-lines[index].a)*qty/2.0+recursive(price,qty+per,index+1,1)+recursive(price,qty-per,index-1,-1);
        }

        double recursive(int pr, int qty, int index, int mul)
        {
            if(index == (numOfLines-1) || index == 0)
                return (pr-lines[index].a) * qty / (slope ? 2.0 : -2.0);

            return (pr-lines[index].a) * qty / 2.0 + recursive(pr, qty+mul*per, index+mul, mul);
        }

        /*double recursives(int left, int right, int middex, int pr, int qty)
        {
            if(left > right)                                   //Even elements: left & right indices cross
                return 0;
            else if(left == numOfLines/2)
                return (lines[left].a-pr)*qty/2.0;            //Odd elements: 1/2 of surplus from left-right similar indices
            else if(left < middex)
                return recursives(pr, qty+per, index+1, 1);
            else if(right > middex)
                return recursives(pr, qty-per, index-1, -1);

            return recursives(left, right+1, pr, qty+per) + recursives(left+1, right, pr, qty-per);
        }

        double recursivo(int pr, int qty, int index, int middex, int mul)        //Data structure Line changes with respect to index change
        {
            if(index == (numOfLines-1) || index == 0)
                return (lines[index].a-pr) * qty / 2.0;
            else if(index < middex)
                return recursivo(pr, qty+per, index+1, middex, 1);
            else if(index > middex)
                return recursivo(pr, qty-per, index-1, middex, -1);

            return (lines[index].a-pr) * qty / 2.0 + recursivo(pr, qty+per, index+1, middex, 1) + recursivo(pr, qty+per, index-1, middex, -1);
        }

        double recursiveToo(int pr, int qty, int index, int mul)
        {
            if(index == (numOfLines-1) || index == 0)
                return ( (lines[index].a-pr) * (qty) / 2.0 );

            return 0 + recursiveToo(pr, qty+mul*per, index+mul, mul);
        }
        */

        double calculateTotalSocial(int quantity)
        {
            printEqns();

            double totalQty = 0.0;
            double startPrice = correspondent(lines[numOfLines-1], true, quantity);

            for(int count = 0; count > (numOfLines-1); count++)
                totalQty += ( correspondent(lines[count], false, startPrice) );
        }

        void MergeSort(Line* lines, int left, int right)
        {
            if(left < right)
            {
                int center = (right + left) / 2;
                MergeSort(lines, left, center);
                MergeSort(lines, center+1, right);
                Merge(lines, left, center, center+1, right);
            }
        }

        void Merge(Line* lines, int left, int leftMid, int rightMid, int right)
        {
            int leftNew = rightMid - left;
            int rightNew = right - leftMid;

            Line* leftArr = new Line[leftNew];
            Line* rightArr = new Line[rightNew];

            int count, counter;
            for(count = 0; count < leftNew; count++)
                leftArr[count] = lines[left+count];

            for(count = 0; count < rightNew; count++)
                rightArr[count] = lines[rightMid+count];

            count = counter = 0; int ptr = left;
            while(count < leftNew && counter < rightNew)
            {
                if(leftArr[count].a <= rightArr[counter].a)
                    lines[ptr++] = leftArr[count++];
                else
                    lines[ptr++] = rightArr[counter++];
            }

            while(count < leftNew)
                lines[ptr++] = leftArr[count++];

            while(counter < rightNew)
                lines[ptr++] = rightArr[counter++];
        }

        void printEqns()
        {
            for(int counter = 0; counter < numOfLines; counter++)
                cout << "Individual " << concern.substr(0,6) << " curve for " << lines[counter].firm << ": p=" << lines[counter].a << (slope ? "+" : "-") << lines[counter].b << "q" << endl;

            cout << endl;
        }
};

int main()
{
    cout << "Enter the market of research: "; string market; getline(cin, market);
    cout << endl << endl;

    cout << "Choose (1) Demand (2) Supply " << endl;
    cout << "Enter your choice: "; int choice; cin >> choice;

    cout << endl;
    cout << "Enter period of adjustment: "; int per; cin >> per;
    cout << "Enter " << ((choice-1) ? "max" : "min") << " quantity: "; int ext; cin >> ext;

    LinearCurves curves( (choice-1) , market, per, ext);

    cout << endl;

    return 0;
}

void QuickSort(int* arr, int low, int high)
{
    if(low >= high)
        return;

    int pivot = partitioning(arr, low, high);
    //Quicksort(arr, low, pivot);
    //Quicksort(arr, pivot+1, high);
}

int partitioning(int* arr, int low, int high)
{
    int ptr = low-1;
    int count;
    for(count = low; count < (high-1); count++)
        if(arr[count] <= arr[high])
            swap(&arr[++low], &arr[count]);

    swap(&arr[count], &arr[high]);

    return (count+1);
}

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}
