Digital E-Wallet (C++ Console Application)

A menu-driven, console-based digital wallet simulator written in standalone C++ (<iostream>, no external libraries). It models a simplified login system, a timed session tracker, an inflow/outflow transaction engine, and a budget-based expense analysis module with category-specific recommendations.

Note: a modern dashboard UI mockup (frontend_landscape.png) exists alongside this code, but the C++ program itself is a terminal application, it has no GUI. The two are not wired together; this README documents what the C++ source actually does.

How the program is organized
The source is split into four "modules":

Module	Responsibility	Key functions
1. Auth	- Login, sign-up, password recovery, input validation -	login, signup, forgotpassword, check_format, masked_pass, cnic_check, get_length
2. Session/Time - A standalone session-timer utility	- time_tracker, timer
3. Transactions	- Cash-in / cash-out flow with per-session limits	- get_amount, show_balance, inflow_process, outflow_process, final_summary
4. Analysis -	Budget vs. spend analysis with canned advice -	expense_analysis

All "state" lives in local variables inside main() and is passed around by reference/value. There's no class, struct, or persistence layer. Nothing is saved to disk. Every run starts from the same hardcoded state.

Module 1: Auth 

Accounts: 3 hardcoded users + 1 signup slot

Authentication isn't backed by a database, it's three accounts baked directly into main():

Email, Password:

abc@gmail.com, a123
abc@nu.edu.pk, b123
abc@hotmail.com, c123

A 4th pair of string references (email4, pass4) acts as a single free "slot" that gets overwritten by whichever account is created via Sign Up, or reset via Forgot Password. This means only one custom account can exist at a time; signing up a second new user silently replaces the first.

Email format whitelist

check_format()
checks whether the string's tail exactly matches one of three hardcoded domain suffixes. It does this with manual character-by-character suffix comparison.
•	@gmail.com (10 chars)
•	@nu.edu.pk (10 chars)
•	@hotmail.com (12 chars)

Masked password display

masked_pass() reflects the entered password back to the screen as a row of * characters, one per character typed, giving the user visual confirmation of password length without redisplaying the actual characters.

CNIC validation

cnic_check() validates that the entered CNIC is exactly 13 digits (reading it as a long long and counting digits via repeated /10). The function returns true to signal "invalid, please re-enter," which is how the call site's while (cnic_check(cnic)) loop keeps prompting until a valid 13-digit number is supplied.

Login retry policy

•	Wrong credentials on first attempt, user is offered "3 (Forgot Password)" or "4 (Try Again)".
•	"Try Again" grants exactly 3 more attempts (for (int i = 0; i <= 2; i++)); failing all three prints "Login Failed. Try again later." and returns without looping back to the main menu, effectively ending the program's auth phase.

Module 2: Time Tracker (a busy-wait timer, not real wallet "session timeout")

This module is disconnected from authentication/session logic, it's invoked from the main menu as option 1, independently of any login session.
•	Asks how many timer "sessions" to queue, then a duration (in minutes) for each.
•	timer(int minutes) counts down via a manual CPU delay loop: 
•	for (int delay = 0; delay < 1000000; delay++) temp++;
used as a one-second delay, repeated minutes * 60 times.
•	Every 600 seconds (10 minutes) it prints a "Time remaining" update; otherwise it's silent until it prints "Session expired. Logging out..." at the end.

Module 3: Transactions - the real wallet engine

Starting balance

Every run starts with a hardcoded balance of PKR 500,000.00 (initial_balance). There is no persistence between runs.

Hard per-session caps

A single "session" (between login and choosing "Terminate Session") is capped at:
•	2 cash-in (inflow) transactions (in_history[2])
•	3 cash-out (outflow) transactions (out_history[3])
These are fixed-size stack arrays so the limits are structural, not just a soft business rule: in_count >= 2 / out_count >= 3 checks gate further input, and once both caps are hit the loop force-exits ("All transaction limits reached for this session").

Up to 3 sessions per login

After "Terminate Session" prints a final_summary(), the user is asked whether to start a new session. The outer do...while loop is capped at session_count >= 3 ("[!] Maximum of 3 login sessions reached for this period"), independent of the per-session transaction caps above. So in a single program run, a user can do at most 3 sessions × (2 inflows + 3 outflows) = 6 inflows and 9 outflows total.

Amount entry is a custom digit-by-digit parser

get_amount() doesn't use cin >> double. It reads one char at a time and builds the number by num = num*10 + digit, terminated by typing ., s, or S. Amounts are entered as whole PKR values. Any non-digit, non-terminator character anywhere in the input sets is_invalid = true, and the function returns sentinel value -1.0, which the caller checks for explicitly to print "FATAL ERROR: Numeric input required."

Inflow categories

Inflow has 4 subcategories (Income/Salary, Cash-In/Self-load, P2P/Personal, Other), printed for confirmation by inflow_process(). in_history[] records the amount for each inflow transaction.

Outflow categories (tracked for real)

Outflow has 8 subcategories mapped 1:1 to the same 8 budget categories used in Module 4 (Food, Grocery, Health, Education, Utilities & Bills, Shopping, Family & Friends, Other). Each outflow:
1.	Increments spent[category] by the amount.
2.	Increments count[category] (transaction counter for that category).
3.	Decrements the running initial_balance.
4.	Is logged with its category index into outflow_sub[] for the session summary.
Insufficient-funds protection exists (amount > initial_balance is rejected before the transaction completes). Budgets set at signup serve as reference points for the analysis module rather than hard transaction caps.

Final summary

final_summary() prints every inflow (amount only) and every outflow (amount and resolved category name, looked up via a category[8][50] array local to the function), plus inflow/outflow counts and the closing balance.

Module 4: Expense Analysis - the most "designed" part of the program

Pre-seeded transaction counts

The 8 categories start with non-zero hardcoded transaction counts: {15, 23, 12, 5, 2, 13, 12, 21} (Food, Grocery, Health, Education, Utilities & Bills, Shopping, Family & Friends, Other). This looks like simulated historical data so that the "not enough transactions" gate (below) doesn't block every fresh run.

Minimum sample size gate

Analysis for a category is only performed if count[index] >= 15. Categories with fewer than 15 recorded transactions just dump raw numbers (budget, spent, count) without generating a status or recommendations. This is why Education (5) and Utilities & Bills (2) are pre-seeded below threshold while Food (15), Grocery (23), Shopping (13, just under), Family (12, under), and Other (21) sit near/at/over it.

Status thresholds

For a category that passes the sample-size gate and has a non-zero budget:
•	Overspent: spent > budget
•	Balanced: budget/2 <= spent <= budget
•	Underspent: spent < budget/2
Each status maps to exactly 3 canned recommendation strings per category, pulled from a hardcoded char[8][3][3][100] lookup table (8 categories × 3 statuses × 3 tips each = 72 total recommendation strings baked into the binary).

Analysis refreshes the category

After printing a full analysis (when there are enough transactions and a budget is set), the function resets count[index] and spent[index] to 0 for that category, clearing it for the next tracking period.

Per-category opt-out

Before running analysis, the user is asked "Do you want to see expense analysis for Category? (Y/N)". Declining still prints the raw budget/spent/count numbers (just skips status + recommendations). It's a verbosity toggle, not a true skip.

Input validation style

Throughout the program, numeric input is never read with cin >> int directly for menu choices, instead the author reads into a char[] buffer and manually parses digits, rejecting anything with non-digit characters or unexpected length. Examples: main menu choice, category selection (1–8), session count and duration in the time tracker, and the Y/N prompts at the end of summaries (which actually check input[1] != '\0', i.e. they require single-character input). This avoids cin failbit issues from typed letters but means every prompt has its own bespoke re-implementation of integer parsing rather than a shared helper.

Color output

ANSI escape codes are used for terminal coloring throughout (RED, GREEN, BLUE, MAGENTA, YELLOW, CYAN, RESET), giving each type of message (errors, success, prompts, headers) a distinct visual style on ANSI-capable terminals.

Build & run

No external dependencies, standard C++ with <iostream> only.
g++ -o ewallet digital_e-wallet.cpp
./ewallet

Sample session walkthrough
1.	Auth: enter 1 to log in with one of the 3 hardcoded accounts (e.g. abc@gmail.com / a123), or 2 to sign up a new (single-slot) account with name, phone number, 13-digit CNIC, email, password, and per-category budgets.
2.	Main menu: 1 time tracker, 2 transactions, 3 expense analysis, 4 exit.
3.	Transactions: select Cash-In or Cash-Out, enter a whole-number amount terminated by . or s, choose a subcategory, repeat up to the session caps, then terminate to see the session summary.
4.	Expense analysis: pick a category 1–8 to see its budget/spend status and (if at least 15 transactions are on record and a budget is set) an overspent/balanced/underspent status along with 3 tailored recommendations.
