#include <iostream>
using namespace std;

const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string YELLOW = "\033[33m";
const std::string CYAN = "\033[46m";
const std::string RESET = "\033[0m";

int login(string &email1, string pass1, string &email2, string pass2, string &email3, string pass3, string &email4, string &pass4);
void signup(string &email4, string &pass4, string &email1, string &email2, string &email3, double budget[]);
void forgotpassword(string email1, string pass1, string email2, string pass2, string email3, string pass3, string &email4, string &pass4);
bool check_format(string &email);
void masked_pass(string password);
bool cnic_check(long long cnic);
int get_length(string &email);

void time_tracker();
void timer(int minutes);

double get_amount();
void show_balance(double balance);
void inflow_process(char sub); 
void outflow_process(char sub);
void final_summary(int in_count, int out_count, double initial_balance, double in_history[], double out_history[], int outflow_sub[]);

void expense_analysis(int index, char category[][50], int count[], double budget[], double spent[]);

int main()
{
    std::cout<<BLUE<<"\n========== DIGITAL E-WALLET ==========\n"<<RESET;
    
    //module 1 variables
    string email1, email2, email3, pass1, pass2, pass3;
    string email4 = " ";
    string pass4 = " ";
    email1 = "abc@gmail.com";
    pass1 = "a123";
    email2 = "abc@nu.edu.pk";
    pass2 = "b123";
    email3 = "abc@hotmail.com";
    pass3 = "c123";
    char input;
    bool valid = false;
    double budget[8] = {0.0};
    
    //module 2 variables
    int choice;
    char input_choice[100];
     
    //module 3 variables
    double initial_balance = 500000.00; 
    double amount;
    char primary_transaction;
    char sub;
    int in_count =0, out_count = 0;
    double in_history[2];
    double out_history[3];
    int outflow_sub[3];
    int session_count = 0;
    double spent[8] = {0.0};
    int count[8] = {15,23,12,5,2,13,12,21}; 
    char exit_system;
    
    //module 4 variables
    char again; 
    char category[8][50] = {"Food","Grocery","Health","Education","Utilities & Bills","Shopping","Family & Friends","Other"};
    
    while(!valid)
    {
        std::cout<<MAGENTA<<"Enter \n1.Login \n2.Sign Up \n3.Forgot Password"<<RESET<<endl;
        cin>>input;
        if(input == '1')
        {
            login(email1, pass1, email2, pass2, email3, pass3, email4, pass4);
            valid = true;
        }
        else if(input == '2')
        {
            signup(email4, pass4, email1, email2, email3, budget);
            valid = true;
        }
        else if(input == '3')
        {
            forgotpassword(email1, pass1, email2, pass2, email3, pass3, email4, pass4);
            valid = true;
        }
        else
        {
            std::cout<<RED<<"Invalid Input. Try again"<<RESET<<endl;
            valid = false;
        }
    }
    
    do{
        std::cout<<MAGENTA<<"\nMAIN MENU\n1. Time tracking\n2. Transactions\n3. Expense analysis\n4. Exit application"<<RESET<<endl;
        cout<<"Enter your choice: ";
        cin>>input_choice;
        
        int length=0;
        while(input_choice[length]!='\0')
        length++;
    if((length==1) && (input_choice[0]>='1') && (input_choice[0]<='4'))
    {
        choice=int(input_choice[0])-48;
    }
    else
    {
        std::cout<<RED<<"Invalid option selected"<<RESET<<endl;
        continue;
    }
    
    if(choice==1)
    {
        time_tracker();
    }
    else if(choice==2)
    {
        do{
            session_count++;
            std::cout<<YELLOW<<"\nWelcome: Session "<< session_count<<" of 3:"<<RESET<<endl;
            
            while (true)
            {
                show_balance(initial_balance);
                std::cout<<MAGENTA<< "\nSelect Transaction Type: \n1. Cash-In (Inflow)\n2. Cash-Out (Outflow)\n3. Terminate Session\nSelection: "<<RESET;
                
                while (true)
                {
                    cin >> primary_transaction;
                    if (primary_transaction >= '1' && primary_transaction <= '3')
                    {
                        break;
                    }
                    else
                    {
                        std::cout<<RED<< "INVALID! Select Primary Transaction: "<<RESET;
                    }
                
                }
                if (primary_transaction == '3')
                {
                    std::cout<<BLUE<< "\nTerminating session... Generating Final Summary..."<<RESET<<endl;
                    break;
                }
                if (primary_transaction == '1')
                {
                    if (in_count >= 2)
                    {
                        std::cout<<RED<<"\nCash-in limit reached."<<RESET<<endl;
                        continue;
                    }
                    
                    while (true)
                    {
                        amount = get_amount(); 
                        if (amount == -1.0)
                        {
                            std::cout<<RED<< "FATAL ERROR: Numeric input required. Try again.\n"<<RESET<<endl;
                        }
                        else if (amount <= 0)
                        {
                            std::cout<<RED<<"INVALID: Enter a positive value.\n"<<RESET<<endl;
                        }
                        else
                        {
                            break;
                        }
                    }
                    
                    std::cout<<MAGENTA<<"\nSelect Inflow Category:\n1. Income/Salary\n2. Cash-In/Self-load\n3. P2P/Personal\n4. Other Inflow\nSelection: "<<RESET;
                    
                    while (true)
                    {
                        cin>>sub;
                        if (sub >= '1' && sub <= '4')
                        {
                            inflow_process(sub);
                            break;
                        }
                        else
                        {
                            std::cout<<RED<< "\nINVALID! Random inputs restricted. Select your subcategory from 1-4: "<<RESET<<endl;
                        }
                    }
                    
                    in_history[in_count] = amount;
                    initial_balance += amount;
                    in_count++;
                    std::cout<<GREEN<< "\nTransaction Successful. Amount Added: PKR "<<amount<<RESET<<endl;
                }
                else if (primary_transaction == '2')
                {
                    if (out_count >= 3)
                    {
                        std::cout<<RED<<"\nCash-out limit reached." <<RESET<<endl;
                        continue;
                    }
                    
                    while (true)
                    {
                        amount = get_amount();
                        if (amount == -1.0)
                        {
                            std::cout<<RED<<"FATAL ERROR: Numeric input required. Try again.\n"<<RESET<<endl;
                        }
                        else if (amount > initial_balance)
                        {
                            std::cout<<RED<<"ERROR! Insufficient funds.\n"<<RESET<<endl;
                        }
                        else if (amount <= 0)
                        {
                            std::cout<<"INVALID! Enter a positive value.\n"<<RESET<<endl;
                        }
                        else
                        {
                            break;
                        }
                    }
                    
                    std::cout<<MAGENTA<<"\nSelect Outflow Category:\n1. Food\n2. Grocery\n3. Health\n4. Education\n5. Utilities/Bills\n6. Shopping\n7. Family/Friends\n8. Other\nSelection: "<<RESET;
                    
                    while (true)
                    {
                        cin>>sub;
                        if (sub >= '1' && sub <= '8')
                        {
                            outflow_process(sub);
                            break;
                        }
                        else
                        {
                            std::cout<<RED<<"\nINVALID! Random inputs restricted. Select your subcategory from 1-8: "<<RESET<<endl;
                        }
                    }
                    
                    int sub_index = (sub - '1'); 
                    out_history[out_count] = amount; 
                    outflow_sub[out_count] = sub_index; 
                    
                    spent[sub_index] += amount;
                    count[sub_index]++;
                    
                    initial_balance -= amount;
                    out_count++;
                    std::cout<<GREEN<<"\nTransaction Successful. Amount Reduced: PKR "<<amount<<RESET<<endl;
                }
                
                if (in_count >= 2 && out_count >= 3) {
                    std::cout<<RED<< "\nAll transaction limits reached for this session."<<RESET<<endl;
                    break;
                }
            }
            
            final_summary(in_count, out_count, initial_balance, in_history, out_history, outflow_sub);
            
            if (session_count >= 3) 
            {
                std::cout<<RED<<"\n[!] Maximum of 3 login sessions reached for this period."<<RESET<<endl;
                break;
            }
            
            std::cout<<YELLOW<< "\nWould you like to login for a new session? (y/n): "<<RESET;
            cin >> exit_system;
        } while (exit_system == 'y' || exit_system == 'Y');
    }
    else if(choice==3)
    {
        std::cout<<BLUE<<"\n========== EXPENSE ANALYSIS: =========="<<RESET<<endl;
        do {
            std::cout<<YELLOW<< "\nExpense Categories:\n"<<RESET;
            
            for (int i = 0; i < 8; i++)
            {
                cout << i + 1<< ". "<< category[i]<< endl;
            }
            int choice;

            while (true)
            {
                std::cout<<YELLOW<<"\nSelect category (1-8): "<<RESET;

                char input[10];
                cin >> input;
                
                bool isNumber = true;
                char *ptr = input;
                
                while (*ptr != '\0')
                {
                    if (*ptr < '0' || *ptr > '9')
                    {
                        isNumber = false;
                        break;
                    }
                    ptr++;
                }
                
                if (!isNumber)
                {
                    std::cout<<RED<<"Invalid choice. Enter numbers only.\n"<<RESET;
                    continue;
                }
                
                choice = 0;
                ptr = input;
                               
                while (*ptr != '\0')
                {
                    choice = choice * 10 + (*ptr - '0');
                    ptr++;
                }
                
                if (choice >= 1 && choice <= 8)
                {
                    break;
                }
                
                std::cout<<RED<<"Invalid choice. Try Again.\n"<<RESET;
            }
            int index = choice - 1;
            expense_analysis(index, category, count, budget, spent);
            
            while (true)
            {
                cout << "\nAnalyze another expense? (Y/N): ";
                char input[10];
                cin >> input;
                
                while(input[1]!='\0')
                {
                    std::cout<<RED<<"Invalid input. Please enter Y or N :"<<RESET;
                    cin>>input;
                }
                again = input[0];
                if (again == 'Y' || again == 'y' || again == 'N' || again == 'n')
                {
                    break;
                }
                else
                {
                    std::cout<<RED<<"Invalid input. Please enter Y or N : "<<RESET;
                }
            }
        } while (again == 'Y' || again == 'y');

        std::cout<<YELLOW<<"\nExpense Analysis Concluded!\n"<<RESET;
    }
    else
    {
        std::cout<<BLUE<<"\nThank you for using Digital Wallet!"<<endl<<RESET;
    }
} while(choice!=4);
return 0;
}

//Module 1: Login/Signup for Digital Wallet
int login(string &email1, string pass1, string &email2, string pass2, string &email3, string pass3, string &email4, string &pass4)
{
    std::cout<<BLUE<<"\n========== LOGIN ==========\n"<<RESET;
    string email, password;
    char input;
    bool valid = false;

    cout<<"Email: ";
    check_format(email);
    cout<<"Password: ";
    cin>>password;
    masked_pass(password);
    if ((email == email1 && password == pass1)||(email == email2 && password == pass2)||(email == email3 && password == pass3)||(email == email4 && password == pass4))
    {
        std::cout<<GREEN<<"\nLogin Successful\n"<<RESET;
    }
    else
    {
        std::cout<<RED<<"\nInvalid Email or Password"<<RESET<<endl;
        std::cout<<MAGENTA<<"Enter 3(Forgot Password) or Enter 4(Try Again)"<<RESET<<endl;
        while(!valid)
        {
            cin>>input;
            if (input == '3')
            {
                forgotpassword(email1, pass1, email2, pass2, email3, pass3, email4, pass4);
                valid = true;
            }
            else if (input == '4')
            {
                for (int i=0; i<=2; i++)
                {
                    cout<<"Email: ";
                    check_format(email);
                    cout<<"Password: ";
                    cin>>password;
                    masked_pass(password);
                    if ((email == email1 && password == pass1)||(email == email2 && password == pass2)||(email == email3 && password == pass3)||(email == email4 && password == pass4))
                    {
                        std::cout<<GREEN<<"\nLogin Successful"<<RESET;
                        break;
                    }
                    if(i!=2)
                    {
                        std::cout<<RED<<"\nLogin Unsuccessfull. Please try again"<<endl<<RESET;
                    }
                    if(i==2)
                    {
                        std::cout<<RED<<"\nLogin Failed. Try again later."<<RESET<<endl;    
                        return 0;
                    }
                }
                valid = true;
            }
            else{
                std::cout<<RED<<"Invalid Input.\n"<<RESET;
                std::cout<<MAGENTA<<"Enter 3(Forgot Password) or 4(Try Again)"<<RESET<<endl;
                valid = false;
            }
        }
    }
    return 0;
}

void signup(string &email4, string &pass4, string &email1, string &email2, string &email3, double budget[])
{
    std::cout<<BLUE<<"\n========== SIGN UP ==========\n"<<RESET;
    
    
    string email, password;
    char category[8][50] = {"Food","Grocery","Health","Education","Utilities & Bills","Shopping","Family & Friends","Other"};
    char name[50];
    long long cnic, number; 
    int i = 0;
    bool valid_name = false;
    bool valid = false;
    
    std::cout<<BLUE<<"\n===== User Information =====\n"<<RESET;
    while (!valid_name)
    {
        cout<<"Enter name: ";
        cin>>name;
        valid_name = true;
        for (int i = 0; name[i] != '\0'; i++)
        {
            if (!((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z')))
            {
                std::cout<<RED<<"Invalid input. Name should only contain letters.\n"<<RESET;
                valid_name = false;
                break;
            }
        }
    }
    cout<<"Enter number: ";
    cin>>number;
    cout<<"Enter cnic(example format : 6110122222227): "<<endl;
    cin>>cnic;
    while(cnic_check(cnic))
    {
        cout<<"Enter cnic(example format : 6110122222227): "<<endl;
        cin>>cnic;
    }
    while(!valid)
    {
        cout<<"Enter email: ";
        check_format(email);
        if((email != email1) && (email != email2) &&(email != email3))
        {
            cout<<"Enter password: ";
            cin>>password;
            masked_pass(password);
            valid = true;
        }
        else
        {
            std::cout<<RED<<"Email already exists. Enter different email"<<RESET<<endl;
            valid = false;
        }
    }
    
    std::cout<<BLUE<<"\n=== EXPENSE BUDGETS ===\n"<<RESET;
    for (int i = 0; i < 8; i++)
    {
        cout << category[i] << ": ";
        cin >> budget[i];
    }
    
    email4 = email;
    pass4 = password;
    std::cout<<GREEN<<"\nAccount Created!\n"<<RESET<<endl;
    
    std::cout<<BLUE<<"\n========== VERIFICATION ==========\n"<<RESET;
    while(true)
    {
        cout<<"Email: ";
        check_format(email);
        cout<<"Password: ";
        cin>>password;
        masked_pass(password);
        
        if (email == email4 && password == pass4)
        {
            std::cout<<GREEN<<"\nVerification Successful"<<RESET;
            break;
        }
        else
        {
            std::cout<<RED<<"\nVerification Failed. Try again\n"<<RESET;
        }
    }
}

void forgotpassword(string email1, string pass1, string email2, string pass2, string email3, string pass3, string &email4, string &pass4)
{
    std::cout<<BLUE<<"\n========== FORGET PASSWORD ==========\n"<<RESET;

    string email, password;
    char input;
    bool valid = false;
    
    cout<<"Enter email: ";
    check_format(email);
    cout<<"Enter New Password: ";
    cin>>password;
    masked_pass(password);
    email4 = email;
    pass4 = password;
    std::cout<<GREEN<<"\nNew Password Set"<<endl<<RESET;
    do
    {
        std::cout<<MAGENTA<<"Enter 1(Login)"<<RESET<<endl;
        cin>>input;
        if (input == '1')
        {
            cout<<"Email: ";
            check_format(email);
            cout<<"Password: ";
            cin>>password;
            masked_pass(password);
            if ((email == email1 && password == pass1)||(email == email2 && password == pass2)||(email == email3 && password == pass3)||(email == email4 && password == pass4))
            {
                std::cout<<GREEN<<"\nLogin Successful\n"<<RESET;
                valid = true;
            }
            else
            {
                std::cout<<RED<<"\nWrong email or password.\n"<<RESET;
            }
        }
        else
        {
            std::cout<<RED<<"\nInvalid Input.\n"<<RESET;
            std::cout<<MAGENTA<<"Enter 1(Login)"<<RESET<<endl;
        }
    } while(!valid);
}

bool check_format(string &email)
{
    const char *gmail = "@gmail.com";
    const char *fast = "@nu.edu.pk";
    const char *hotmail = "@hotmail.com";
    
    while (true)
    {
        cin>>email;
        
        int lenEmail = get_length(email); 
        const int lenGmail = 10;
        const int lenFast = 10;
        const int lenHotmail = 12;
        bool isGmail = false;
        bool isFast = false;
        bool isHotmail = false;
        
        if (lenEmail >= lenGmail)
        {
            isGmail = true;
            int j = 0;
            for (int i= lenEmail - lenGmail; i< lenEmail; i++)
            {
                if (email[i] != gmail[j++])
                {
                    isGmail = false;
                    break;
                }
            }
        }
        if (lenEmail >= lenFast)
        {
            isFast = true;
            int j = 0;
            for (int i = lenEmail - lenFast; i < lenEmail; i++)
            {
                if (email[i] != fast[j++])
                {
                    isFast = false;
                    break;
                }
            }
        }
        if (lenEmail >= lenHotmail)
        {
            isHotmail = true;
            int j = 0;
            for (int i= lenEmail - lenHotmail; i < lenEmail; i++)
            {
                if (email[i] != hotmail[j++])
                {
                    isHotmail = false;
                    break;
                }
            }
        }
        if (isGmail || isFast || isHotmail)
        {
            return true;
        }
        else
        {
            std::cout<<RED<< "Invalid Format. Use (@gmail.com/@nu.edu.pk/@hotmail.com)"<<RESET<<endl;
            cout<<"Email: ";
        }
    }
}

void masked_pass(string password)
{
    int len = 0;
    int i = 0;
    
    while (password[len] != '\0')
    {
        len++;
    }
    cout<<"Password: ";
    while(i < len)
    {
        cout<<"*";
        i++;
    }
    cout<<endl;
}

bool cnic_check(long long cnic)
{
    int len = 0;
    long long temp = cnic;
    
    if(temp == 0)
    {
        return true;
    }
    while(temp!=0)
    {
        temp = temp/10;
        len++;
    }
    if (len == 13)
    {
        return false;
    }
    else
    {
        std::cout<<RED<< "Invalid CNIC. Must be 13 digits."<<RESET<<endl;
        return true;
    }
}

int get_length(string &email)
{
    int len = 0;
    int i = 0;
    
    while(email[i] != '\0')
    {
        len++;
        i++;
    }
    return len;
}

//Module 2: Time/Session to remain Login in Digital Wallet
void time_tracker()
{
    char sessionInput[100];
    int sessions;
    
    std::cout<<YELLOW<<"\nEnter number of sessions: "<<RESET;
    cin>>sessionInput;
    
    int sessionLen=0;
    while(sessionInput[sessionLen]!='\0')
    {
        sessionLen++;
    }
    bool validSession=true;
    sessions=0;
    
    for(int j=0;j<sessionLen;j++)
    {
        if(((sessionInput[j])<='0') || (sessionInput[j]>='9'))
        {
            validSession=false;
            break;
        }
        sessions = sessions * 10 + (int(sessionInput[j])- 48); 
    }
    if((!validSession) || sessions<1)
    {
        std::cout<<RED<<"Invalid number of sessions"<<RESET<<endl;
        return;
    }
    int times[100];
    for (int i=0; i<sessions; i++)
    {
        char input[100];
        int minutes=0;
        bool valid=false;
        while (!valid)
        {
            std::cout<<YELLOW<<"\nEnter time (minutes) for session "<<i+1<<": "<<RESET;
            cin>>input;
            minutes=0;
            valid=true;
            for(int j=0;input[j]!='\0';j++)
            {
                if(input[j]<'0'||input[j]>'9')
                {
                    valid=false;
                    break;
                }
                minutes=minutes*10+(int(input[j])-48);
            }
            if(minutes<1)
            valid=false;
        if(!valid)
        {
            std::cout<<RED<<"Invalid time. Please enter again."<<RESET<<endl;
        }
    }
    times[i]=minutes;
}

for (int i=0;i<sessions;i++)
{
    cout<<"Starting Session "<<i+1<<endl;
    timer(times[i]);
}
}

void timer(int minutes)
{
    int totalseconds=minutes*60;
    int temp=0;
    for(int secspassed=0;secspassed<totalseconds;secspassed++)
    {
        for(int delay=0;delay<1000000;delay++)
        {
            temp++;
        }
        if(secspassed!=0 && secspassed % 600==0)
        {
            std::cout<<YELLOW<<"Time remaining: "<<(totalseconds-secspassed)<<" seconds"<<RESET<<endl;
        }
    }
    std::cout<<RED<<"Session expired. Logging out..."<<RESET<<endl;
}
//Module 3: Basic Transactions
double get_amount()
{
    double num = 0;
    char ch;
    bool is_invalid = false;
    
    std::cout<<YELLOW<< "Enter amount (Follow by '.' or 's' to finish): "<<RESET;
    while (cin >> ch)
    {
        if (ch >= '0' && ch <= '9')
        {
            num = (num * 10) + (ch - '0');
        }
        else if (ch == '.' || ch == 's' || ch == 'S')
        {
            break;
        }
        else 
        {
            is_invalid = true;
        }
    }
    if (is_invalid) 
    {
        return -1.0; 
    }
    return num; 
}


void show_balance(double balance) 
{
    cout << "\n==============================\n";
    cout << "Current Balance: PKR " << balance << endl;
}

void inflow_process(char sub) 
{
    if (sub == '1')
    {
        cout << "Categorized: Income/Salary" << endl;
    }
    else if (sub == '2')
    {
        cout << "Categorized: Cash-In/Self-load" << endl;
    }
    else if (sub == '3')
    {
        cout << "Categorized: P2P/Personal" << endl;
    }
    else
    {
        cout << "Categorized: Other Inflow" << endl;
    }
}

void outflow_process(char sub) 
{
    if (sub == '1')
    {
        cout << "Store in: Food" << endl;
    }
    else if (sub == '2')
    {
        cout << "Store in: Grocery" << endl;
    }
    else if (sub == '3')
    {
        cout << "Store in: Health" << endl;
    }
    else if (sub == '4')
    {
        cout << "Store in: Education" << endl;
    }
    else if (sub == '5')
    {
        cout << "Store in: Utilities/Bills" << endl;
    }
    else if (sub == '6')
    {
        cout << "Store in: Shopping" << endl;
    }
    else if (sub == '7')
    {
        cout << "Store in: Family & Friends" << endl;
    }
    else
    {
        cout << "Store in: Other" << endl;
    }
}

void final_summary(int in_count, int out_count, double initial_balance, double in_history[], double out_history[], int outflow_sub[]) 
{
    std::cout<<BLUE<<"\n===Final Session Summary ==="<<RESET<<endl;
    std::cout<<MAGENTA<< "Inflow Transactions Details:"<<RESET<<endl;
    for (int i = 0; i < in_count; i++)
    {
        cout << " [" << i + 1 << "] Amount: PKR " << in_history[i] << endl;
    }

    std::cout<<MAGENTA<<"\nOutflow Transactions Details:"<<RESET<<endl;
    char category[8][50] = {"Food","Grocery","Health","Education","Utilities & Bills","Shopping","Family & Friends","Other"};
    for (int i = 0; i < out_count; i++)
    {
        cout << " [" << i + 1 << "] Amount: PKR " << out_history[i] << " ("<<category[outflow_sub[i]]<< ")" << endl;
    }
    std::cout<<CYAN<< "Total Successful Inflows: " << in_count << endl;
    cout << "Total Successful Outflows: " << out_count << endl;
    
    cout << "Final Closing Balance: PKR " << initial_balance << endl;
    cout << "\nSession Closed."<<RESET<< endl;
}

///Module 4: Expense Analysis
void expense_analysis(int index, char category[][50], int count[], double budget[], double spent[])
{
    char recommendations[8][3][3][100] =
    {
        //Food
        {
            // Overspent
            {
                "Reduce restaurant or fast-food orders and prefer home-cooked meals.",
                "Set a weekly limit for food delivery apps.",
                "Track daily food expenses to avoid frequent small purchases."
            },
            // Balanced
            {
                "Maintain the current spending pattern.",
                "Continue tracking food orders weekly.",
                "Occasionally cook at home to maintain balance."
            },
            // Underspent
            {
                "Ensure proper nutrition and regular meals.",

                "Allocate a small budget for occasional dining.",
                "Avoid skipping meals to save money."
            }
        },
        //Grocery
        {
            // Overspent
            {
                "Make a grocery list before shopping.",
                "Avoid buying unnecessary or bulk items.",
                "Compare prices between local stores and supermarkets."
            },
            // Balanced
            {
                "Continue planned grocery shopping.",
                "Buy essentials in reasonable quantities.",
                "Take advantage of discounts when available."
            },
            // Underspent
            {
                "Ensure essential household items are stocked.",
                "Maintain a balanced diet with proper groceries.",
                "Plan monthly grocery purchases properly."
            }
        },
        //Health
        {
            // Overspent
            {
                "Review medical expenses and avoid unnecessary medicines.",
                "Consider health insurance or medical coverage.",
                "Maintain a healthy lifestyle to reduce doctor visits."
            },
            // Balanced
            {
                "Continue regular health checkups when needed.",
                "Maintain preventive healthcare habits."
                "Keep emergency medical funds."
            },
            // Underspent
            {
                "Do not ignore necessary medical checkups.",
                "Allocate funds for health emergencies.",
                "Maintain regular physical checkups."
            }
        },
        //Education
        {
            // Overspent
            {
                "Review unnecessary course or subscription expenses.",
                "Use free educational resources when possible.",
                "Plan tuition and learning costs in advance."
            },
            // Balanced
            {
                "Continue investing in useful learning resources.",
                "Maintain a budget for books and courses.",
                "Track educational spending each semester."
            },
            // Underspent
            {
                "Consider investing in skill-development courses.",
                "Purchase essential study materials.",
                "Allocate funds for learning opportunities."
            }
        },
        //Bills
        {
            // Overspent
            {
                "Reduce electricity and gas consumption.",
                "Turn off unused appliances and lights.",
                "Monitor monthly bills regularly."
            },
            // Balanced
            {
                "Maintain current energy usage habits.",
                "Continue paying bills on time.",
                "Track seasonal changes in utility expenses."
            },
            // Underspent
            {
                "Ensure all bills are paid properly.",
                "Allocate funds for unexpected utility increases.",
                "Maintain consistent bill tracking."
            }
        },
        //Shopping
        {
            // Overspent
            {
                "Avoid impulse purchases and unnecessary items.",
                "Set a monthly shopping limit.",
                "Wait before making non-essential purchases."
            },
            // Balanced
            {
                "Continue controlled shopping habits.",
                "Plan purchases before visiting stores.",
                "Prefer quality over quantity."
            },
            // Underspent
            {
                "Replace essential items when needed.",
                "Plan necessary purchases periodically.",
                "Maintain a small allowance for personal items."
            }
        },
        //Family
        {
            // Overspent
            {
                "Limit frequent social gatherings or expensive outings.",
                "Plan budgets for gifts or events.",
                "Balance social spending with personal finances."
            },
            // Balanced
            {
                "Continue supporting family responsibly.",
                "Maintain a planned budget for social activities.",
                "Track shared expenses when going out."
            },
            // Underspent
            {
                "Allocate some funds for social connections.",
                "Participate occasionally in family gatherings.",
                "Plan small budgets for celebrations or gifts."
            }
        },
        //Others
        {
            // Overspent
            {
                "Review miscellaneous expenses regularly.",
                "Avoid unnecessary small purchases.",
                "Categorize expenses properly to track them."
            },
            // Balanced
            {
                "Maintain a small flexible spending budget.",
                "Monitor unexpected expenses.",
                "Keep records for miscellaneous spending."
            },
            // Underspent
            {
                "Maintain an emergency or miscellaneous fund.",
                "Allocate funds for unexpected expenses.",
                "Continue tracking minor transactions."
            }
        }
    };
    char response;
    char input[10];

    while(true)
    {
        std::cout<<YELLOW<<"\nDo you want to see expense analysis for "<<category[index]<<" ? (Y/N) : "<<RESET;
        cin>>input;
        while(input[1] != '\0')
        {
            std::cout<<RED<<"Invalid input. Enter Y or N : "<<RESET;
            cin>>input;
        }
        response=input[0];
        if(response == 'Y' || response == 'y' || response == 'N' || response == 'n')
        {
            break;
        }
        else
        {
            std::cout<<RED<<"Invalid input. Enter Y or N: "<<RESET;
        }
    }
    if (response == 'N' || response == 'n')
    {
        std::cout<<CYAN<< "\n=== Expense Analysis Skipped for " << category[index]<< " ===\n"<<endl;
        cout << "Budget for " << category[index]<< " = " <<budget[index]<<endl;
        cout << "Spending for " << category[index]<< " = " <<spent[index]<<endl;
        cout << "Number of transactions for " << category[index]<< " = " <<count[index]<<RESET<<endl;
    }
    else
    {
        if (count[index] < 15)
        {
            std::cout<<CYAN<< "\n=== Not enough transactions for analysis in "<< category[index]<< " ===\n"<<endl;
            cout << "Budget for " << category[index]<< " = " <<budget[index]<<endl;
            cout << "Spending for " << category[index]<< " = " <<spent[index]<<endl;
            cout << "Number of transactions for " << category[index]<< " = " <<count[index]<<endl<<RESET;
        }
        else if (budget[index] == 0)
        {
            std::cout<<RED<<"\nNo budget assigned for "<< category[index]<<RESET<<endl;
        }
        else
        {
            double percent_used =(spent[index] / budget[index]) * 100;
            double remaining = budget[index] - spent[index];
            if(remaining<0)
            {
                remaining = 0;
            }
            std::cout<<CYAN<< "\n=== Expense Analysis for "<< category[index]<< " ===\n"<<endl;
            cout << "Budget for " << category[index]<<" = PKR "<< budget[index]<< endl;
            cout << "Spent for " << category[index]<< " = PKR "<< spent[index]<< endl;
            cout << "Remaining for " << category[index]<< " = PKR "<< remaining<< endl;
            cout << "Budget Used for " << category[index]<< " = PKR "<< percent_used<< "%\n"<<RESET;
            
            int status_index;
            if (spent[index] > budget[index])
            {
                status_index = 0;
                std::cout<<RED<< "\nStatus: OVERSPENT"<<endl;
                cout << "Over Budget By: PKR "<< (spent[index] - budget[index])<<RESET<<endl;
            }
            else if (spent[index] >= budget[index] / 2.0)
            {
                status_index = 1;
                std::cout<<GREEN<<"\nStatus: BALANCED SPENDING\n"<<RESET;
            }
            else
            {
                status_index = 2;
                std::cout<<GREEN<<"\nStatus: UNDERSPENT\n"<<RESET;
            }
            
            std::cout<<YELLOW<<"\nRecommendations for "<<category[index]<<" :"<<endl<<RESET;
            for (int i = 0; i < 3; i++)
            {
                cout<<"- "<<recommendations[index][status_index][i]<<endl;
            }
            count[index]=0;
            spent[index]=0;
        }
    }
}