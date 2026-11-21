#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<float.h>
#include<limits.h>

#define name_length 50
#define dept_length 30
#define pass_length 20
#define double_lim 999999999
#define full_time 8
#define part_time 4

#define ADMIN_PASSWORD "admin123"        //password for the admin portal
#define DEFAULT_EMP_PASSWORD "emp123"    //Here emp is the name of employee like ali123, ahmed123, anus123 etc
#define MAX_ATTEMPTS 3                   //wrong password attempts allowed
#define LINE "======================================================================"
#define DASH "----------------------------------------------------------------------"

#ifdef _WIN32
int _getch(void);   //used to hide the password while typing (Windows console)
#endif

const char *month_names[12] = {"January","February","March","April","May","June",
							   "July","August","September","October","November","December"};

typedef struct{//salary structure
	double basic;
	double health_all;
	double travelling_all;
	double house_all;
	double hourly_rate;
	double contract_pay;
}salary;

typedef struct{//deductions structure
	double loan;
	double loan_installment;
	double other_deductions;
}deductions;

typedef struct{//employee structure
	int id;
	char name[name_length];
	char department[dept_length];
	char password[pass_length];//login password of the employee
	int employee_type;
	int att[4][5];
	salary sal;
	deductions ded;
	double overtime_hours;
	double basic_earned,allownces,overtime_pay,gross;
	double tax,loan_ded,net;
}employee;

typedef struct{//one payroll record = one employee for one month (this is what gets stored in history)
	int id;
	char name[name_length];
	char department[dept_length];
	int employee_type;
	int month,year;
	int present_days;
	double overtime_hours;
	double base_rate;//basic salary / hourly rate / contract pay
	double health,house,travel;
	double basic_earned,overtime_pay,gross;
	double tax,loan_ded,other_ded,net;
	double loan_balance;//loan remaining after that month
}record;

typedef struct{//dynamic array of all payroll records
	record *data;
	int count;
	int limit;
}history;



int readInt(const char*prompt,int lo,int hi){//read any integer with validation
	char line[100]; int res=-1;
	printf("\n%s:",prompt);
	for(;;){
	fgets(line,100,stdin);
	sscanf(line,"%d",&res);
	if(res>=lo && res<hi)
		return res;
	else
		printf("Invalid input, must be in between %d and %d\n",lo,hi);
	}
}

void readtext(const char*prompt,char*buf,int n){//reads any string with validations
	printf("\n%s: ",prompt);
	for(;;){
		fgets(buf,n,stdin);
		buf[strcspn(buf,"\n")]='\0';
		if(buf[0]=='\0') printf("This field cannot be empty.\n");
		else if(strchr(buf,',')!=NULL) printf("Commas (,) are not allowed in this field.\n");//comma would break the CSV files
		else return;
	}
}

double readDouble(const char*prompt,double low,double high){//reads any float value with validations
	char line[100]; double v=-1.0;
	printf("\n%s: ",prompt);
	for(;;){
		fgets(line,sizeof(line),stdin);
		sscanf(line,"%lf",&v);
		if(v>=low && v<high) return v;
		else printf("Invalid input,must be in between %.2lf and %.2lf\n",low,high);
	}
}

//reads a password (shows * on Windows console, plain text on other systems)
void readPassword(const char *prompt,char *buf,int n){
#ifdef _WIN32
	int i,ch;
	for(;;){
		printf("\n%s: ",prompt);
		fflush(stdout);
		i=0;
		while((ch=_getch())!='\r' && ch!='\n'){
			if(ch==0 || ch==224){ _getch(); continue; }//ignore arrow/function keys
			if(ch=='\b'){
				if(i>0){ i--; printf("\b \b"); fflush(stdout); }
			}
			else if(ch>=32 && ch<127 && i<n-1){
				buf[i++]=(char)ch;
				printf("*");
				fflush(stdout);
			}
		}
		buf[i]='\0';
		printf("\n");
		if(i>0) return;
		printf("This field cannot be empty.\n");
	}
#else
	readtext(prompt,buf,n);
#endif
}

//checks the rules for a NEW password
int valid_password(const char *p){
	int i,len=(int)strlen(p);
	if(len<4 || len>=pass_length){
		printf("Password must be between 4 and %d characters.\n",pass_length-1);
		return 0;
	}
	for(i=0;i<len;i++){
		if(isspace((unsigned char)p[i]) || p[i]==','){
			printf("Password cannot contain spaces or commas.\n");
			return 0;
		}
	}
	return 1;
}

//asks until a valid new password is entered (masked = 1 hides the typing)
void get_password(const char *prompt,char *dest,int masked){
	char tmp[100];
	for(;;){
		if(masked) readPassword(prompt,tmp,sizeof(tmp));
		else readtext(prompt,tmp,sizeof(tmp));
		if(valid_password(tmp)){
			strcpy(dest,tmp);
			return;
		}
	}
}

//prints a heading line of total width 70 with the title in the middle
void banner(const char *title){
	int len=(int)strlen(title),left=(70-len-2)/2,right=70-len-2-left,i;
	if(left<0) left=0;
	if(right<0) right=0;
	for(i=0;i<left;i++) putchar('=');
	printf(" %s ",title);
	for(i=0;i<right;i++) putchar('=');
	putchar('\n');
}

const char *type_name(int t){
	if(t==1) return "Full time";
	if(t==2) return "Part time";
	return "Contract";
}

//recursive function
int find_employee(employee E[],int n,int id,int index){

	if(index == n){//check till till the employee count
		printf("Id not found!.\n");
		return -1;
	}
	if(E[index].id == id){//base condition
		return index;
	}
	return find_employee(E,n,id,index+1);//recursive part
}

int flag(const char *c){
	char a[10];
	printf("%s (y/n): ",c);
	for(;;){
	fgets(a,sizeof(a),stdin);
	if(a[0] == 'y' || a[0] == 'Y') return 1;
	else if(a[0] == 'n' || a[0] == 'N') return 0;
	else
		printf("Invalid Input!. Enter (y/n)\n");
	}
}



//dynamic memory allocation function to increase the size of employee array
void maintain_Memory(employee **E,int *employee_limit){
	*employee_limit *= 2;
	*E = (employee*)realloc(*E,*employee_limit*sizeof(employee));
	if(*E==NULL){
		printf("Memory allocation failed!\n");
		exit(1);
	}
}

//function to add employees
int addemployee(employee E[],int n){
	int i;
	memset(&E[n],0,sizeof(employee));//clean slate (attendance, payroll fields start from 0)
	E[n].id=readInt("Enter employee id",1,9999999);
		for(i=0;i<n;i++){//checks for duplicate id
					if(E[i].id==E[n].id){
						printf("Id already exists\n");
						return n;
					}
				}
	readtext("Enter employee name",E[n].name,sizeof(E[n].name));
	readtext("Enter employee department",E[n].department,sizeof(E[n].department));
	printf("\nSet a login password for this employee (4-%d characters, no spaces or commas).",pass_length-1);
	get_password("Enter employee password",E[n].password,0);
	E[n].employee_type=readInt("1) Full time 2) Part time 3) Contract ",1,4);
	switch(E[n].employee_type){
		case 1:
			E[n].sal.basic=readDouble("Enter basic salary",1,double_lim);
			E[n].sal.health_all=readDouble("Enter Health allowances",0,double_lim);
			E[n].sal.house_all=readDouble("Enter House allownces",0,double_lim);
			E[n].sal.travelling_all=readDouble("Enter travelling allownces",0,double_lim);

			break;
		case 2:
			E[n].sal.hourly_rate=readDouble("Enter hourly rate",1,double_lim);
			break;
		case 3:
			E[n].sal.contract_pay=readDouble("Enter Fixed monthly contract pay",1,double_lim);
			break;
	}
	E[n].ded.loan=readDouble("Enter loan if any",0,double_lim);
				if(E[n].ded.loan>0)
					E[n].ded.loan_installment=readDouble("Enter monthly loan installments",1,E[n].ded.loan);
				else
					E[n].ded.loan_installment = 0;
	E[n].ded.other_deductions=readDouble("Any other deductions",0,double_lim);
	E[n].overtime_hours = 0;
	n++;
	printf("\nEmployee added succesfully!\n");
	return n;
}

//function to update employee details
void update(employee *P){
	printf("Enter what to upadate: \n");
	int read;
	int check = readInt("1)Basic info 2)Salary and allownces 3)Deductions and loans 4)Login password",1,5);
	switch(check){
		case 1:
		read  = readInt("1) Name 2) Department 3)Employee type",1,4);
				switch(read){
				case 1:
				readtext("Enter new name",P->name,sizeof(P->name));
				break;
				case 2:
				readtext("Enter new department name",P->department,sizeof(P->department));
				break;
				case 3:
				P->employee_type=readInt("1) Full time 2) Part time 3) Contract ",1,4);
					switch(P->employee_type){
						case 1:
							P->sal.basic=readDouble("Enter basic salary",1,double_lim);
							P->sal.health_all=readDouble("Enter Health allowances",0,double_lim);
							P->sal.house_all=readDouble("Enter House allownces",0,double_lim);
							P->sal.travelling_all=readDouble("Enter travelling allownces",0,double_lim);

							break;
						case 2:
							P->sal.hourly_rate=readDouble("Enter hourly rate",1,double_lim);
							break;
						case 3:
							P->sal.contract_pay=readDouble("Enter Fixed monthly contract pay",1,double_lim);
							break;
					}
					break;
			}
		break;
		case 2:
		switch(P->employee_type){
						case 1:
							read = readInt("1) Basic salary 2) Health allowances 3) House allownces 4) Travelling allownces",1,5);
							switch(read){
								case 1:
									P->sal.basic=readDouble("Enter new basic salary",1,double_lim);
									break;
								case 2:
									P->sal.health_all=readDouble("Enter new Health allowances",0,double_lim);
									break;
								case 3:
									P->sal.house_all=readDouble("Enter new House allownces",0,double_lim);
									break;
								case 4:
									P->sal.travelling_all=readDouble("Enter new travelling allownces",0,double_lim);
									break;
							}
							break;
						case 2:
							P->sal.hourly_rate=readDouble("Enter new hourly rate",1,double_lim);
							break;
						case 3:
							P->sal.contract_pay=readDouble("Enter new Fixed monthly contract pay",1,double_lim);
							break;
					}
		break;
		case 3:
		read = readInt("1) Loan 2) Loan installments 3) Other deductions",1,4);
		switch(read){
			case 1:
				P->ded.loan=readDouble("Enter new loan amount",0,double_lim);
					if(P->ded.loan > 0){
						P->ded.loan_installment = readDouble("Enter new loan installments",1,P->ded.loan);
					}
					else
						P->ded.loan_installment = 0;
				break;
			case 2:
			if(P->ded.loan>0)
				P->ded.loan_installment=readDouble("Enter new monthly loan installments",1,P->ded.loan);
			else
				printf("No loan amount to update installments\n");
				break;
			case 3:
				P->ded.other_deductions=readDouble("Enter new other deductions",0,double_lim);
				break;
		}
		break;
		case 4:
			get_password("Enter new password for this employee",P->password,0);
			printf("Password changed.\n");
			break;
	}
}

//function to delete employee from the list
void delete(employee E[],int index,int *n){
	int i;
	for(i=index;i<*n-1;i++){
		E[i]=E[i+1];
	}
	(*n)--;
	printf("\nEmployee deleted successfully!\n");
}

//update and delete function
void upd_and_del(employee E[],int *n){
	int choice = readInt("1)Update 2)Delete",1,3);
	int temp_id,index;
	switch(choice){
		case 1:
			temp_id = readInt("Enter employee id to update",1,9999999);
			index = find_employee(E,*n,temp_id,0);
			if(index==-1 ) return;
			update(&E[index]);
			printf("\nEmployee updated successfully!\n");
			break;
		case 2:
			temp_id = readInt("Enter employee id to delete",1,9999999);
			index = find_employee(E,*n,temp_id,0);
			if(index==-1) return;
			if(flag("Are you sure you want to delete this employee"))
				delete(E,index,n);
			break;
	}
}

//employee header function to display employee details
void employee_header(employee E[],int index,int n){
	const char *rate_label;
	double rate;
	if(E[index].employee_type==1){ rate_label="Basic salary"; rate=E[index].sal.basic; }
	else if(E[index].employee_type==2){ rate_label="Hourly rate"; rate=E[index].sal.hourly_rate; }
	else{ rate_label="Contract pay"; rate=E[index].sal.contract_pay; }
	printf("\n%s\n",LINE);
	printf("%-13s: %-20.20s %-12s: %-20.20s\n","Name",E[index].name,"Department",E[index].department);
	printf("%-13s: %-20s %-12s: %-20d\n","Employee type",type_name(E[index].employee_type),"Employee ID",E[index].id);
	printf("%-13s: %-20.2lf %-12s: %-20.2lf\n",rate_label,rate,"Loan",E[index].ded.loan);
	printf("%s\n",LINE);
}

//function to display employee list
void employee_list(employee E[],int n){
	int i;
	printf("\n");
	banner("EMPLOYEE LIST");
	if(n==0){
		printf("No employees added yet.\n");
		return;
	}
	for(i=0;i<n;i++){
		employee_header(E,i,n);
	}
}

//function to mark attendance for an employee
void Marks_attendence(employee E[],int n){
	int temp_id,index;
	temp_id = readInt("Enter employee id to mark attendance",1,9999999);
	index = find_employee(E,n,temp_id,0);
	if(index==-1) return;
	int day = readInt("Enter day of the week (1-5) where 1=Mon, 2=Tue, 3=Wed, 4=Thu, 5=Fri",1,6);
	int week = readInt("Enter week number (1-4)",1,5);
	E[index].att[week-1][day-1] = 1;
}

//function to mark overtime for an employee
void mark_overtime(employee E[],int n){
	int temp_id,index;
	temp_id = readInt("Enter employee id to mark overtime",1,9999999);
	index = find_employee(E,n,temp_id,0);
	if(index==-1) return;
	if(E[index].employee_type==3){
		printf("Contract employees are not eligible for overtime.\n");
		return;
	}
	double hours = readDouble("Enter number of overtime hours",0,double_lim);
	E[index].overtime_hours += hours;
}

void Att_and_OT(employee E[],int n){
	if(n==0){
		printf("No employees added yet.\n");
		return;
	}
	int choice = readInt("1) Mark attendance 2) Mark overtime",1,3);
	switch(choice){
		case 1:
			Marks_attendence(E,n);
			break;
		case 2:
			mark_overtime(E,n);
			break;
	}
}

//prints the attendance matrix of the employee at position index
void print_attendance_of(employee E[],int index){
	int i,j;
	printf("\n");
	banner("ATTENDANCE MATRIX (CURRENT MONTH)");
	printf("%-13s: %-20.20s %-12s: %-20d\n","Name",E[index].name,"Employee ID",E[index].id);
	printf("%s\n",DASH);
	printf("%-8s","Week");
	printf("Mon\tTue\tWed\tThu\tFri\n");
	for(i=0;i<4;i++){
		printf("%-8d",i+1);
		for(j=0;j<5;j++){
			if(E[index].att[i][j]==1) printf("P\t");
			else printf("A\t");
		}
		printf("\n");
	}
	printf("%s\n",LINE);
}

//function to print attendance matrix for an employee
void print_attendance(employee E[],int n){

	int temp_id,index;
	temp_id = readInt("Enter employee id to view attendance",1,9999999);
	index = find_employee(E,n,temp_id,0);
	if(index==-1) return;
	print_attendance_of(E,index);
}

//function to search employee by id
void search_employee(employee E[],int n){
	int temp_id,index;
	if(n==0){
		printf("No employees to search.\n");
		return;
	}
	temp_id = readInt("Enter employee id to search",1,9999999);
	index = find_employee(E,n,temp_id,0);
	if(index==-1) return;
	employee_header(E,index,n);
}

//function to calculate the number of present days for an employee
int present_days(employee E[],int n,int index){
	int i,j,count=0;
	for(i=0;i<4;i++){
		for(j=0;j<5;j++){
			if(E[index].att[i][j]==1) count++;
		}
	}
	return count;
}

//function to calculate payroll for all employees
void payroll(employee E[],int n){
	int i;
	for(i=0;i<n;i++){
		if(E[i].employee_type==1){
			double per_day_salary = E[i].sal.basic/20;
			E[i].basic_earned = present_days(E,n,i)*per_day_salary;
			E[i].allownces = E[i].sal.health_all + E[i].sal.house_all + E[i].sal.travelling_all;
			E[i].overtime_pay = E[i].overtime_hours * 1.5 * E[i].sal.basic/(full_time*20);
		}
		else if(E[i].employee_type==2){
			double per_day_salary = E[i].sal.hourly_rate * part_time;
			E[i].basic_earned = present_days(E,n,i) * per_day_salary;
			E[i].allownces = 0;
			E[i].overtime_pay = E[i].overtime_hours *1.5 * E[i].sal.hourly_rate;
		}
		else{
			double per_day_salary = E[i].sal.contract_pay/20;
			E[i].basic_earned = present_days(E,n,i) * per_day_salary;
			E[i].allownces = 0;
			E[i].overtime_pay = 0;
		}
		E[i].gross = E[i].basic_earned + E[i].allownces + E[i].overtime_pay;

		//progressive tax calculation
		if(E[i].gross <= 50000)
			E[i].tax = 0.1 * E[i].gross;
		else if(E[i].gross <= 100000)
			E[i].tax = 0.1*50000 + 0.2 * (E[i].gross - 50000);
		else if(E[i].gross <= 200000)
			E[i].tax = 0.1*50000 + 0.2*50000 + 0.25 * (E[i].gross - 100000);
		else
			E[i].tax = 0.1*50000 + 0.2*50000 + 0.25*100000 + 0.3 * (E[i].gross - 200000);
		if(E[i].ded.loan_installment > E[i].ded.loan)//if the loan installment is greater than the remaining loan, deduct only the remaining loan
			E[i].loan_ded = E[i].ded.loan;
		else
			E[i].loan_ded = E[i].ded.loan_installment;

		 if(E[i].loan_ded > E[i].gross){//if the loan installment is greater than the gross salary, deduct only the gross salary
			E[i].loan_ded = E[i].gross-E[i].tax;}
			E[i].ded.loan -= E[i].loan_ded;//deducting the loan installment from the remaining loan
		E[i].net = E[i].gross - (E[i].tax + E[i].loan_ded + E[i].ded.other_deductions);
		memset(E[i].att,0,sizeof(E[i].att));//resetting attendance for next month
		E[i].overtime_hours = 0;//resetting overtime hours for next month
	}
}



/* ===================== PAYROLL HISTORY (month wise records) ===================== */

void history_init(history *H){
	H->count = 0;
	H->limit = 12;
	H->data = (record*)malloc(H->limit*sizeof(record));
	if(H->data==NULL){
		printf("Memory allocation failed!\n");
		exit(1);
	}
}

int history_key(const record *r){//year+month as one number, used for ordering
	return r->year*100 + r->month;
}

//adds a record and keeps the history sorted by year and month
void history_add(history *H,const record *r){
	int i;
	if(H->count >= H->limit){
		record *tmp = (record*)realloc(H->data,H->limit*2*sizeof(record));
		if(tmp==NULL){
			printf("Memory allocation failed! A history record could not be stored.\n");
			return;
		}
		H->data = tmp;
		H->limit *= 2;
	}
	i = H->count;
	H->data[i] = *r;
	H->count++;
	while(i>0 && history_key(&H->data[i-1]) > history_key(&H->data[i])){
		record t = H->data[i-1];
		H->data[i-1] = H->data[i];
		H->data[i] = t;
		i--;
	}
}

//returns index of the record of this employee for this month, or -1
int history_find(history *H,int id,int month,int year){
	int i;
	for(i=0;i<H->count;i++){
		if(H->data[i].id==id && H->data[i].month==month && H->data[i].year==year)
			return i;
	}
	return -1;
}

//returns 1 if payroll was already processed for this month
int history_month_exists(history *H,int month,int year){
	int i;
	for(i=0;i<H->count;i++){
		if(H->data[i].month==month && H->data[i].year==year)
			return 1;
	}
	return 0;
}

//returns index of the most recent record of an employee, or -1
int history_latest(history *H,int id){
	int i;
	for(i=H->count-1;i>=0;i--){
		if(H->data[i].id==id) return i;
	}
	return -1;
}

//prints the months for which this employee has a record, returns how many
int history_list_months(history *H,int id){
	int i,count=0;
	for(i=0;i<H->count;i++){
		if(H->data[i].id==id){
			printf("%s%.3s %d",(count==0)?"\nRecords available for: ":", ",month_names[H->data[i].month-1],H->data[i].year);
			count++;
		}
	}
	if(count>0) printf("\n");
	return count;
}

void history_save(history *H){
	int i;
	FILE *P = fopen("History.csv","w");
	if(P==NULL){
		printf("Error! Could not open History.csv for writing.\n");
		return;
	}
	fputs("Year,Month,Id,Name,Department,Employee_type,Present_days,Overtime_hours,Base_rate,Health,House,Travel,Basic_earned,Overtime_pay,Gross,Tax,Loan_ded,Other_ded,Net,Loan_balance,\n",P);
	for(i=0;i<H->count;i++){
		record *r = &H->data[i];
		fprintf(P,"%d,%d,%d,%s,%s,%d,%d,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,\n",
			r->year,r->month,r->id,r->name,r->department,r->employee_type,r->present_days,r->overtime_hours,
			r->base_rate,r->health,r->house,r->travel,r->basic_earned,r->overtime_pay,r->gross,
			r->tax,r->loan_ded,r->other_ded,r->net,r->loan_balance);
	}
	fclose(P);
}

static char *next_token(void){//next CSV field ("0" if the line ended early)
	char *t = strtok(NULL,",");
	return (t!=NULL) ? t : (char*)"0";
}

void history_load(history *H){
	char line[800];
	FILE *P = fopen("History.csv","r");
	if(P==NULL) return;//no history yet
	fgets(line,sizeof(line),P);//skip header
	while(fgets(line,sizeof(line),P)!=NULL){
		record r;
		char *tok;
		line[strcspn(line,"\r\n")]='\0';
		if(line[0]=='\0') continue;
		memset(&r,0,sizeof(r));
		tok = strtok(line,",");
		r.year = atoi(tok);
		r.month = atoi(next_token());
		r.id = atoi(next_token());
		strncpy(r.name,next_token(),name_length-1);
		strncpy(r.department,next_token(),dept_length-1);
		r.employee_type = atoi(next_token());
		r.present_days = atoi(next_token());
		r.overtime_hours = atof(next_token());
		r.base_rate = atof(next_token());
		r.health = atof(next_token());
		r.house = atof(next_token());
		r.travel = atof(next_token());
		r.basic_earned = atof(next_token());
		r.overtime_pay = atof(next_token());
		r.gross = atof(next_token());
		r.tax = atof(next_token());
		r.loan_ded = atof(next_token());
		r.other_ded = atof(next_token());
		r.net = atof(next_token());
		r.loan_balance = atof(next_token());
		if(r.month<1 || r.month>12 || r.year<2000) continue;//skip damaged lines
		history_add(H,&r);
	}
	fclose(P);
}

static void payslip_row(const char *label,double value){
	printf("%-48s %21.2lf\n",label,value);
}

//prints one payslip from a stored record
void payslip_record(const record *r){
	char title[80];
	int pad;
	const char *rate_label;
	if(r->employee_type==1) rate_label="Basic salary";
	else if(r->employee_type==2) rate_label="Hourly rate";
	else rate_label="Contract pay";
	printf("\n%s\n",LINE);
	printf("%-13s: %-20.20s %-12s: %-20.20s\n","Name",r->name,"Department",r->department);
	printf("%-13s: %-20s %-12s: %-20d\n","Employee type",type_name(r->employee_type),"Employee ID",r->id);
	printf("%-13s: %-20.2lf %-12s: %-20.2lf\n",rate_label,r->base_rate,"Loan left",r->loan_balance);
	printf("%-13s: %-20d %-12s: %-20.1lf\n","Days present",r->present_days,"Overtime hrs",r->overtime_hours);
	printf("%s\n",LINE);
	sprintf(title,"PAYSLIP - %s %d",month_names[r->month-1],r->year);
	pad = (70-(int)strlen(title))/2;
	if(pad<0) pad=0;
	printf("%*s%s\n",pad,"",title);
	printf("%s\n",LINE);
	printf("%-48s %21s\n","Earnings","Amount");
	printf("%s\n",DASH);
	payslip_row("Basic Salary",r->basic_earned);
	if(r->employee_type==1){
		payslip_row("Health Allowances",r->health);
		payslip_row("House Allowances",r->house);
		payslip_row("Travelling Allowances",r->travel);
	}
	payslip_row("Overtime Pay",r->overtime_pay);
	printf("%s\n",DASH);
	payslip_row("Gross Salary",r->gross);
	printf("%s\n",DASH);
	printf("%-48s %21s\n","Deductions","Amount");
	printf("%s\n",DASH);
	payslip_row("Tax Deducted",r->tax);
	payslip_row("Loan Deducted",r->loan_ded);
	payslip_row("Other Deductions",r->other_ded);
	printf("%s\n",DASH);
	payslip_row("Total Deductions",r->tax + r->loan_ded + r->other_ded);
	printf("%s\n",DASH);
	payslip_row("Net Salary",r->net);
	printf("%s\n",LINE);
}

//admin: print the payslip of any employee for any saved month
void Print_payslip(history *H){
	int id,month,year,idx;
	if(H->count==0){
		printf("No payroll has been run yet, so there are no payslips.\n");
		return;
	}
	id = readInt("Enter employee id to print payslip",1,9999999);
	if(history_list_months(H,id)==0){
		printf("No payslips found for this employee id.\n");
		return;
	}
	month = readInt("Enter month (1-12)",1,13);
	year = readInt("Enter year",2000,2101);
	idx = history_find(H,id,month,year);
	if(idx==-1){
		printf("No payslip found for %s %d.\n",month_names[month-1],year);
		return;
	}
	payslip_record(&H->data[idx]);
}

//month wise table + summary of one employee (used by admin and by the employee portal)
void history_table(history *H,int id){
	int i,months=0,total_days=0;
	double t_gross=0,t_tax=0,t_net=0,t_ot=0;
	char label[30];
	const char *name=NULL;
	for(i=0;i<H->count;i++){
		if(H->data[i].id==id){ name=H->data[i].name; }
	}
	if(name==NULL){
		printf("No payroll history found for this employee id.\n");
		return;
	}
	printf("\n");
	banner("PAYROLL HISTORY");
	printf("Employee: %s (ID %d)\n",name,id);
	printf("%s\n",DASH);
	printf("%-9s %-5s %-8s %-12s %-12s %-12s\n","Month","Days","OT hrs","Gross","Tax","Net");
	printf("%s\n",DASH);
	for(i=0;i<H->count;i++){
		record *r=&H->data[i];
		if(r->id!=id) continue;
		sprintf(label,"%.3s %d",month_names[r->month-1],r->year);
		printf("%-9s %-5d %-8.1lf %-12.2lf %-12.2lf %-12.2lf\n",label,r->present_days,r->overtime_hours,r->gross,r->tax,r->net);
		months++;
		total_days += r->present_days;
		t_gross += r->gross;
		t_tax += r->tax;
		t_net += r->net;
		t_ot += r->overtime_hours;
	}
	printf("%s\n",DASH);
	printf("%-48s %21d\n","Months on record",months);
	printf("%-48s %21.2lf\n","Total gross earned",t_gross);
	printf("%-48s %21.2lf\n","Total tax paid",t_tax);
	printf("%-48s %21.2lf\n","Total net received",t_net);
	printf("%-48s %21.2lf\n","Average net per month",t_net/months);
	printf("%-48s %21.1lf\n","Average days present (out of 20)",(double)total_days/months);
	printf("%-48s %21.1lf\n","Total overtime hours",t_ot);
	printf("%s\n",LINE);
}

//admin: payroll history of one employee
void View_history(history *H){
	int id;
	if(H->count==0){
		printf("No payroll has been run yet, so there is no history.\n");
		return;
	}
	id = readInt("Enter employee id to view payroll history",1,9999999);
	history_table(H,id);
}

//admin: report of all employees for one month
void monthly_report(history *H){
	int i,month,year,count=0;
	double t_gross=0,t_tax=0,t_loan=0,t_other=0,t_net=0;
	char title[60];
	if(H->count==0){
		printf("No payroll has been run yet, so there are no monthly reports.\n");
		return;
	}
	month = readInt("Enter month (1-12)",1,13);
	year = readInt("Enter year",2000,2101);
	if(!history_month_exists(H,month,year)){
		printf("No payroll was processed for %s %d.\n",month_names[month-1],year);
		printf("Processed months: ");
		for(i=0;i<H->count;i++){
			if(i==0 || H->data[i].month!=H->data[i-1].month || H->data[i].year!=H->data[i-1].year)
				printf("%s%.3s %d",(i==0)?"":", ",month_names[H->data[i].month-1],H->data[i].year);
		}
		printf("\n");
		return;
	}
	sprintf(title,"PAYROLL REPORT - %s %d",month_names[month-1],year);
	printf("\n");
	banner(title);
	printf("%-7s %-18s %-12s %-4s %-12s %-12s\n","Id","Name","Department","Days","Gross","Net");
	printf("%s\n",DASH);
	for(i=0;i<H->count;i++){
		record *r=&H->data[i];
		if(r->month!=month || r->year!=year) continue;
		printf("%-7d %-18.18s %-12.12s %-4d %-12.2lf %-12.2lf\n",r->id,r->name,r->department,r->present_days,r->gross,r->net);
		t_gross += r->gross;
		t_tax += r->tax;
		t_loan += r->loan_ded;
		t_other += r->other_ded;
		t_net += r->net;
		count++;
	}
	printf("%s\n",DASH);
	printf("%-48s %21d\n","Employees paid",count);
	printf("%-48s %21.2lf\n","Total gross salary",t_gross);
	printf("%-48s %21.2lf\n","Total tax deducted",t_tax);
	printf("%-48s %21.2lf\n","Total loan deducted",t_loan);
	printf("%-48s %21.2lf\n","Total other deductions",t_other);
	printf("%-48s %21.2lf\n","Total net salary",t_net);
	printf("%s\n",LINE);
}

void collect_dept(employee E[],int n,char dept[][dept_length],int *dept_count){
	int i,j,found;
	*dept_count = 0;
	for(i=0;i<n;i++){
		found = 0;
		for(j=0;j<*dept_count;j++){
			if(strcmp(E[i].department,dept[j])==0){
				found = 1;
				break;
			}
		}
		if(!found){
			strcpy(dept[*dept_count],E[i].department);
			(*dept_count)++;
		}
	}
}

void Departmental_report(employee E[],int n,int *dept_count,char dept[][dept_length]){
	int i,j;
	collect_dept(E,n,dept,dept_count);
	printf("\n");
	banner("DEPARTMENTAL REPORT (LATEST PAYROLL)");
	if(n==0){
		printf("No employees added yet.\n");
		return;
	}
	for(i=0;i<*dept_count;i++){
		double total_gross=0,total_tax=0,total_loan=0,total_other=0,total_net=0;
		int full_time_count=0,part_time_count=0,contract_count=0;
		for(j=0;j<n;j++){
			if(strcmp(E[j].department,dept[i])==0){
				total_gross += E[j].gross;
				total_tax += E[j].tax;
				total_loan += E[j].loan_ded;
				total_other += E[j].ded.other_deductions;
				total_net += E[j].net;
				if(E[j].employee_type==1) full_time_count++;
				else if(E[j].employee_type==2) part_time_count++;
				else if(E[j].employee_type==3) contract_count++;
			}
		}
		printf("%-12s : %s\n","Department",dept[i]);
		printf("%s\n",DASH);
		printf("%-48s %21.2lf\n","Total Gross Salary",total_gross);
		printf("%-48s %21.2lf\n","Total Tax Deducted",total_tax);
		printf("%-48s %21.2lf\n","Total Loan Deducted",total_loan);
		printf("%-48s %21.2lf\n","Total Other Deductions",total_other);
		printf("%-48s %21.2lf\n","Total Net Salary",total_net);
		printf("%-48s %21d\n","Full-time Employees",full_time_count);
		printf("%-48s %21d\n","Part-time Employees",part_time_count);
		printf("%-48s %21d\n",	"Contract Employees",contract_count);
		printf("%s\n",LINE);
	}
}


void write(employee E[],int n){
	FILE *P = NULL;
	P = fopen("EMPLOYEE.csv","w");
	if(P == NULL){
		printf("Error! Could not open EMPLOYEE.csv for writing.\n");
		return ;
	}
	int i;
	fputs("Name,Id,Department,Employee_type,Basic_salary,Health_allowances,House_allowances,Travelling_allowances,Hourly_rate,Contract_pay,Loan,Loan_installments,Other_deductions,Password,",P);
	fprintf(P,"\n");
	for(i = 0 ;i<n; i++){
	fprintf(P,"%s,%d,%s,",E[i].name,E[i].id,E[i].department);
	if(E[i].employee_type==1) fprintf(P,"%s,%.2lf,%.2lf,%.2lf,%.2lf,%c,%c,","Full_Time",E[i].sal.basic,E[i].sal.health_all,E[i].sal.house_all,E[i].sal.travelling_all,'-','-');
	else if(E[i].employee_type == 2) fprintf(P,"%s,%c,%c,%c,%c,%.2lf,%c,","Part_Time",'-','-','-','-',E[i].sal.hourly_rate,'-');
	else fprintf(P,"%s,%c,%c,%c,%c,%c,%.2lf,","Contract",'-','-','-','-','-',E[i].sal.contract_pay);
	fprintf(P,"%.2lf,%.2lf,%.2lf,%s,",E[i].ded.loan,E[i].ded.loan_installment,E[i].ded.other_deductions,E[i].password);

	fprintf(P,"\n");
	}
	fclose(P);

	P = fopen("Attendance.csv","w");
	if(P == NULL){
		printf("Error! Couldnot open Attendance,csv File.\n");
		return;
	}
	int j,k;
	fputs("Id,Day1,Day2,Day3,Day4,Day5,Day6,Day7,Day8,Day9,Day10,Day11,Day12,Day13,Day14,Day15,Day16,Day17,Day18,Day19,Day20",P);
	fprintf(P,"\n");
	for(i=0;i<n;i++){
		fprintf(P,"%d,",E[i].id);
	for(j = 0 ;j<4;j++)
		for(k = 0;k<5;k++)
			if(E[i].att[j][k]==1)
				fputs("P,",P);
			else
				fputs("A,",P);
		fprintf(P,"\n");
	}
	fclose(P);

	P = fopen("Payroll.csv","w");
	if(P == NULL){
		printf("Error! Couldnot open Payroll.csv File");
		return;
	}
	fputs("Id,Basic_earned,Allowances,Overtime_hours,Overtime_pay,Gross,Tax,loan_ded,Net",P);
	fprintf(P,"\n");
	for(i = 0 ; i<n ; i++){
		fprintf(P,"%d,",E[i].id);
		if(E[i].employee_type == 1){
			fprintf(P,"%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,",E[i].basic_earned,E[i].allownces,E[i].overtime_hours,E[i].overtime_pay,E[i].gross);
			fprintf(P,"%.2lf,%.2lf,%.2lf,",E[i].tax,E[i].loan_ded,E[i].net);}
		else if(E[i].employee_type == 2){
			fprintf(P,"%.2lf,%c,%.2lf,%.2lf,%.2lf,",E[i].basic_earned,'-',E[i].overtime_hours,E[i].overtime_pay,E[i].gross);
			fprintf(P,"%.2lf,%.2lf,%.2lf,",E[i].tax,E[i].loan_ded,E[i].net);
		}
		else{
			fprintf(P,"%.2lf,%c,%c,%c,%.2lf,",E[i].basic_earned,'-','-','-',E[i].gross);
			fprintf(P,"%.2lf,%.2lf,%.2lf,",E[i].tax,E[i].loan_ded,E[i].net);
		}
		fprintf(P,"\n");
	}
	fclose(P);
}

void read(employee **K,int *n,int *employee_limit){
	char line[300];
	FILE *P = NULL;
	P = fopen("EMPLOYEE.csv","r");
	if(P==NULL){
		printf("No saved employee data found.\n");
		return;}
	*n=0;
	fgets(line,sizeof(line),P);

	employee *E = *K;

	int i = 0;
	while(fgets(line,sizeof(line),P)!=NULL){
		line[strcspn(line,"\r\n")]='\0';
		if(line[0]=='\0') continue;
		if(*n >= *employee_limit){
			maintain_Memory(K,employee_limit);
			E = *K;}

			memset(&E[i],0,sizeof(employee));//clean slate before filling this employee
			char *tok = strtok(line,",");
			strcpy(E[i].name,tok);

			tok = strtok(NULL,",");
			E[i].id = atoi(tok);

			tok = strtok(NULL,",");
			strcpy(E[i].department,tok);

			tok = strtok(NULL,",");
			if(strcmp(tok,"Full_Time")==0)
				E[i].employee_type = 1;
			else if(strcmp(tok,"Part_Time")==0)
				E[i].employee_type = 2;
			else
				E[i].employee_type = 3;

			tok = strtok(NULL,",");
			if(tok[0]!='-') E[i].sal.basic = atof(tok);

			tok = strtok(NULL,",");
			if(tok[0]!='-') E[i].sal.health_all = atof(tok);

			tok = strtok(NULL,",");
			if(tok[0]!='-') E[i].sal.house_all = atof(tok);

			tok = strtok(NULL,",");
			if(tok[0]!='-') E[i].sal.travelling_all = atof(tok);

			tok = strtok(NULL,",");
			if(tok[0]!='-') E[i].sal.hourly_rate = atof(tok);

			tok = strtok(NULL,",");
			if(tok[0]!='-') E[i].sal.contract_pay = atof(tok);

			tok = strtok(NULL,",");
			E[i].ded.loan = atof(tok);

			tok = strtok(NULL,",");
			E[i].ded.loan_installment = atof(tok);

			tok = strtok(NULL,",");
			E[i].ded.other_deductions = atof(tok);

			tok = strtok(NULL,",");//password (older files do not have this column)
			if(tok!=NULL){
				strncpy(E[i].password,tok,pass_length-1);
				E[i].password[pass_length-1]='\0';
			}
			else
				strcpy(E[i].password,DEFAULT_EMP_PASSWORD);
			(*n)++;
			i++;

	}fclose(P);

	P = fopen("Attendance.csv","r");
	if(P==NULL){
		printf("No saved employee data found.\n");
		return;}

	fgets(line,sizeof(line),P);
	int j,k = 0;
	while(fgets(line,sizeof(line),P)!=NULL){
		line[strcspn(line,"\n")]='\0';
		if(line[0]=='\0') continue;

		char *tok = strtok(line,",");

		for(i = 0;i<4;i++){
			for(j = 0;j<5;j++){
				tok = strtok(NULL,",");
				E[k].att[i][j] = (tok[0]=='P')?1:0;
			}
		}
		k++;
	}
	fclose(P);

	P = fopen("Payroll.csv","r");
	if(P==NULL){
		printf("No saved employee data found.\n");
		return;}

	fgets(line,sizeof(line),P);
	i = 0;
	while(fgets(line,sizeof(line),P)!=NULL){
		line[strcspn(line,"\n")]='\0';
		if(line[0]=='\0') continue;

		char *tok = strtok(line,",");
		tok = strtok(NULL,",");

		E[i].basic_earned = atof(tok);

		tok = strtok(NULL,",");
		E[i].allownces = (tok[0]=='-')?0:atof(tok);

		tok = strtok(NULL,",");
		E[i].overtime_hours = (tok[0]=='-')?0:atof(tok);

		tok = strtok(NULL,",");
		E[i].overtime_pay = atof(tok);

		tok = strtok(NULL,",");
		E[i].gross = atof(tok);

		tok = strtok(NULL,",");
		E[i].tax = atof(tok);

		tok = strtok(NULL,",");
		E[i].loan_ded = atof(tok);

		tok = strtok(NULL,",");
		E[i].net = atof(tok);
		i++;
	}
	fclose(P);
}



//shows who has how many present days before the payroll is run
void attendance_summary(employee E[],int n,int month,int year){
	int i,d,none=0;
	char title[80];
	sprintf(title,"ATTENDANCE SUMMARY - %s %d",month_names[month-1],year);
	printf("\n");
	banner(title);
	printf("%-8s %-20s %-10s %-6s %-8s\n","Id","Name","Type","Days","OT hrs");
	printf("%s\n",DASH);
	for(i=0;i<n;i++){
		d = present_days(E,n,i);
		if(d==0) none++;
		printf("%-8d %-20.20s %-10s %-6d %-8.1lf\n",E[i].id,E[i].name,type_name(E[i].employee_type),d,E[i].overtime_hours);
	}
	printf("%s\n",DASH);
	if(none>0)
		printf("Warning: %d employee(s) have no attendance marked (their basic pay will be zero).\n",none);
}

//runs payroll for a chosen month and stores every employee's result in the history
void run_payroll(employee E[],int n,history *H){
	int i,month,year;
	int *days;
	double *ot;
	if(n==0){
		printf("No employees to run payroll for.\n");
		return;
	}
	month = readInt("Enter payroll month (1-12)",1,13);
	year = readInt("Enter payroll year",2000,2101);
	if(history_month_exists(H,month,year)){
		printf("\nPayroll for %s %d has already been processed and saved.\n",month_names[month-1],year);
		printf("It cannot be run twice (loans and deductions would be applied again).\n");
		return;
	}
	while(flag("Do you want to mark attendance or overtime before running payroll"))
		Att_and_OT(E,n);
	attendance_summary(E,n,month,year);
	if(!flag("Proceed with payroll for this month")){
		printf("Payroll cancelled.\n");
		return;
	}
	days = (int*)malloc(n*sizeof(int));
	ot = (double*)malloc(n*sizeof(double));
	if(days==NULL || ot==NULL){
		printf("Memory allocation failed!\n");
		free(days);
		free(ot);
		return;
	}
	for(i=0;i<n;i++){//attendance and overtime are reset by payroll(), so keep a copy first
		days[i] = present_days(E,n,i);
		ot[i] = E[i].overtime_hours;
	}
	payroll(E,n);
	for(i=0;i<n;i++){
		record r;
		memset(&r,0,sizeof(r));
		r.id = E[i].id;
		strcpy(r.name,E[i].name);
		strcpy(r.department,E[i].department);
		r.employee_type = E[i].employee_type;
		r.month = month;
		r.year = year;
		r.present_days = days[i];
		r.overtime_hours = ot[i];
		if(E[i].employee_type==1){
			r.base_rate = E[i].sal.basic;
			r.health = E[i].sal.health_all;
			r.house = E[i].sal.house_all;
			r.travel = E[i].sal.travelling_all;
		}
		else if(E[i].employee_type==2)
			r.base_rate = E[i].sal.hourly_rate;
		else
			r.base_rate = E[i].sal.contract_pay;
		r.basic_earned = E[i].basic_earned;
		r.overtime_pay = E[i].overtime_pay;
		r.gross = E[i].gross;
		r.tax = E[i].tax;
		r.loan_ded = E[i].loan_ded;
		r.other_ded = E[i].ded.other_deductions;
		r.net = E[i].net;
		r.loan_balance = E[i].ded.loan;
		history_add(H,&r);
	}
	free(days);
	free(ot);
	history_save(H);
	write(E,n);//also saves the employee, attendance and payroll files
	printf("\nPayroll for %s %d processed for %d employee(s).\n",month_names[month-1],year,n);
	printf("Records saved in History.csv (all previous months are kept).\n");
}



/* ===================== LOGIN AND MENUS ===================== */

void menu(){//admin menu
	printf("\n");
	banner("ADMIN PORTAL");
	printf("%-37s%s\n","1) Add employee"               ,"6) Search employee by id");
	printf("%-37s%s\n","2) List employees"             ,"7) Run monthly payroll");
	printf("%-37s%s\n","3) Update or delete employees" ,"8) Print payslip (any month)");
	printf("%-37s%s\n","4) Attendance and overtime"    ,"9) Departmental report");
	printf("%-37s%s\n","5) View attendance matrix"     ,"10) Save or load data from file");
	printf("%-37s%s\n","11) Payroll history of an employee","12) Monthly payroll report");
	printf("0) Logout\n");
	printf("%s\n",LINE);
}

void employee_menu(){//employee menu
	printf("\n");
	banner("EMPLOYEE PORTAL");
	printf("1) View my latest payslip\n");
	printf("2) View payslip of a previous month\n");
	printf("3) View my payroll history\n");
	printf("4) View my attendance (current month)\n");
	printf("5) Change my password\n");
	printf("0) Logout\n");
	printf("%s\n",LINE);
}

//asks for the admin password, returns 1 if correct
int admin_login(void){
	char pw[100];
	int attempt;
	for(attempt=1;attempt<=MAX_ATTEMPTS;attempt++){
		readPassword("Enter admin password",pw,sizeof(pw));
		if(strcmp(pw,ADMIN_PASSWORD)==0){
			printf("\nAdmin login successful.\n");
			return 1;
		}
		printf("Wrong password. %d attempt(s) left.\n",MAX_ATTEMPTS-attempt);
	}
	printf("Too many wrong attempts. Returning to the main screen.\n");
	return 0;
}

//asks for employee id and password, returns the employee index or -1
int employee_login(employee E[],int n){
	char pw[100];
	int id,index,attempt;
	if(n==0){
		printf("\nNo employees are registered yet. Please ask the admin to add you.\n");
		return -1;
	}
	id = readInt("Enter your employee id",1,9999999);
	index = find_employee(E,n,id,0);
	if(index==-1) return -1;
	for(attempt=1;attempt<=MAX_ATTEMPTS;attempt++){
		readPassword("Enter your password",pw,sizeof(pw));
		if(strcmp(pw,E[index].password)==0)
			return index;
		printf("Wrong password. %d attempt(s) left.\n",MAX_ATTEMPTS-attempt);
	}
	printf("Too many wrong attempts. Returning to the main screen.\n");
	return -1;
}

//employee changes his own password
void change_password(employee E[],int n,int index){
	char cur[100],p1[100],p2[100];
	readPassword("Enter your current password",cur,sizeof(cur));
	if(strcmp(cur,E[index].password)!=0){
		printf("Wrong current password. Password not changed.\n");
		return;
	}
	get_password("Enter new password (4-19 characters, no spaces or commas)",p1,1);
	readPassword("Confirm new password",p2,sizeof(p2));
	if(strcmp(p1,p2)!=0){
		printf("Passwords do not match. Password not changed.\n");
		return;
	}
	strcpy(E[index].password,p1);
	write(E,n);//saved immediately so the new password is not lost
	printf("Password changed successfully.\n");
}

//what an employee can do after logging in
void employee_portal(employee E[],int n,int index,history *H){
	int choice,idx,month,year;
	int id = E[index].id;
	printf("\nWelcome, %s!\n",E[index].name);
	do{
		employee_menu();
		choice = readInt("Enter your choice",0,6);
		switch(choice){
			case 1:
				idx = history_latest(H,id);
				if(idx==-1) printf("No payslip is available yet. Payroll has not been run for you.\n");
				else payslip_record(&H->data[idx]);
				break;
			case 2:
				if(history_list_months(H,id)==0){
					printf("No payslips are available yet.\n");
					break;
				}
				month = readInt("Enter month (1-12)",1,13);
				year = readInt("Enter year",2000,2101);
				idx = history_find(H,id,month,year);
				if(idx==-1) printf("No payslip found for %s %d.\n",month_names[month-1],year);
				else payslip_record(&H->data[idx]);
				break;
			case 3:
				history_table(H,id);
				break;
			case 4:
				print_attendance_of(E,index);
				break;
			case 5:
				change_password(E,n,index);
				break;
			case 0:
				printf("Logged out.\n");
				break;
		}
	}while(choice!=0);
}

//full access portal of the admin (the old main menu)
void admin_portal(employee **E,int *employee_count,int *employee_limit,history *H){
	int dept_count = 0;
	char department[100][dept_length];
	do{
		menu();
		int choice = readInt("Enter your choice",0,13);
		switch(choice){
			case 1:
			if(*employee_count >= *employee_limit){
				maintain_Memory(E,employee_limit);}
				*employee_count = addemployee(*E,*employee_count);
				break;
			case 2:
				employee_list(*E,*employee_count);
				break;
			case 3:
				upd_and_del(*E,employee_count);
				break;
			case 4:
				Att_and_OT(*E,*employee_count);
				break;
			case 5:
				print_attendance(*E,*employee_count);
				break;
			case 6:
				search_employee(*E,*employee_count);
				break;
			case 7:
				if(flag("Are you sure you want to run monthly payroll"))
				run_payroll(*E,*employee_count,H);
				break;
			case 8:
				Print_payslip(H);
				break;
			case 9:
				Departmental_report(*E,*employee_count,&dept_count, department);
				break;
			case 10:{
				int C = readInt("1)Save Data  2)Load Data",1,3);
				switch(C){
					case 1:
					if(flag("Do you want to save data to File"))
						write(*E,*employee_count);
					break;
					case 2:
					if(flag("Do you want load data from File"))
						read(E,employee_count,employee_limit);
					break;
				}
				break;}
			case 11:
				View_history(H);
				break;
			case 12:
				monthly_report(H);
				break;
			case 0:
				if(flag("Are you sure you want to logout")){
					if(flag("Save all data to files before logging out"))
						write(*E,*employee_count);
					printf("Admin logged out.\n");
					return;}
				break;
			default:
				printf("Invalid choice. Please try again.\n");
		}
	} while(1);
}

int main()
{
	int employee_count=0;
	int employee_limit = 2;
	history H;
	FILE *chk;
	employee *E=(employee*)malloc(employee_limit*sizeof(employee));
	if(E==NULL){
    printf("Memory allocation failed!\n");
    exit(1);
}
	history_init(&H);

	chk = fopen("EMPLOYEE.csv","r");//load the saved data automatically if it exists
	if(chk!=NULL){
		fclose(chk);
		read(&E,&employee_count,&employee_limit);
		printf("Saved data loaded (%d employee(s)).\n",employee_count);
	}
	history_load(&H);

	do{
		printf("\n");
		banner("EMPLOYEE PAYROLL SYSTEM");
		printf("1) Admin login\n");
		printf("2) Employee login\n");
		printf("0) Exit program\n");
		printf("%s\n",LINE);
		int role = readInt("Enter your choice",0,3);
		switch(role){
			case 1:
				if(admin_login())
					admin_portal(&E,&employee_count,&employee_limit,&H);
				break;
			case 2:{
				int index = employee_login(E,employee_count);
				if(index!=-1)
					employee_portal(E,employee_count,index,&H);
				break;}
			case 0:
				if(flag("Are you sure you want to exit")){
				printf("Exiting the program.\n");
				free(E);
				free(H.data);
				return 0;}
				break;
		}
	} while(1);

}
