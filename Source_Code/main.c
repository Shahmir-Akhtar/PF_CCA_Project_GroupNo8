#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define name_length 50
#define dept_length 30
#define double_lim 999999999
#define full_time 8
#define part_time 4



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
	int employee_type;
	int att[4][5];
	salary sal;
	deductions ded;
	double overtime_hours;
	double basic_earned,allownces,overtime_pay,gross;
	double tax,loan_ded,net;
}employee;



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
		if(buf[0]!='\0') return;
		printf("This field cannot be empty.\n");
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

//recursive function
int find_employee(employee E[],int n,int id,int index){
	
	if(index == n){//check till the employee count
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
	E[n].id=readInt("Enter employee id",1,9999999);
		for(i=0;i<n;i++){//checks for duplicate id
					if(E[i].id==E[n].id){
						printf("Id already exists\n");
						return n;
					}
				}
	readtext("Enter employee name",E[n].name,sizeof(E[n].name));
	readtext("Enter employee department",E[n].department,sizeof(E[n].department));
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
	int check = readInt("1)Basic info 2)Salary and allownces 3)Deductions and loans",1,5);
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

//employee header function to display employee detailsf
void employee_header(employee E[],int index,int n){
	printf("\n======================================================================\n");
	printf("%-12s :%-30s %-12s :%-30s\n","Name",E[index].name,"Department",E[index].department);
	if(E[index].employee_type==1){
		printf("%-12s:%-30s %-12s :%-30d\n","Employee type","Full time","Employee ID",E[index].id);
		printf("%-12s :%-30.2lf %-12s :%-30.2lf","Basic salary",E[index].sal.basic,"Loan",E[index].ded.loan);
		}
	else if(E[index].employee_type==2){
		printf("%-12s:%-30s %-12s :%-30d\n","Employee type","Part time","Employee ID",E[index].id);
		printf("%-12s :%-30.2lf %-12s :%-30.2lf","Hourly rate",E[index].sal.hourly_rate,"Loan",E[index].ded.loan);
		}
	else{
		printf("%-12s:%-30s %-12s :%-30d\n","Employee type","Contract","Employee ID",E[index].id);
		printf("%-12s :%-30.2lf %-12s :%-30.2lf","Contract pay",E[index].sal.contract_pay,"Loan",E[index].ded.loan);
	}
	printf("\n======================================================================\n");
}

//function to display employee list
void employee_list(employee E[],int n){
	int i;
	printf("\n========================EMPLOYEE LIST=================================\n");
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

//function to print attendance matrix for an employee
void print_attendance(employee E[],int n){

	int temp_id,index;
	temp_id = readInt("Enter employee id to view attendance",1,9999999);
	index = find_employee(E,n,temp_id,0);
	if(index==-1) return;
	int i,j;
	printf("\n========================Attendance Matrix=============================\n");
	printf("\n%-12s :%-30s %-12s :%-30d\n","Name",E[index].name,"Employee ID",E[index].id);
		printf("Mon\tTue\tWed\tThu\tFri\n");
	for(i=0;i<4;i++){
		for(j=0;j<5;j++){
			if(E[index].att[i][j]==1) printf("P\t");
			else printf("A\t");
		}
		printf("\n");
	}
	printf("\n======================================================================\n");
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

void Print_payslip(employee E[],int n){
	int temp_id,index;
	if(n==0){
		printf("No employees to print payslip.\n");
		return;
	}
	temp_id = readInt("Enter employee id to print payslip",1,9999999);
	index = find_employee(E,n,temp_id,0);
	if(index==-1) return;
	employee_header(E,index,n);
	printf("======================================================================\n");
	printf("                               PAYSLIP                                \n");
	printf("======================================================================\n");
	printf("%-56s %-30s","Earnings","Amount");
	printf("\n----------------------------------------------------------------------");
	printf("\n%-56s %-30.2lf","Basic Salary",E[index].basic_earned);
	if(E[index].employee_type==1){
		printf("\n%-56s %-30.2lf","Health Allowances",E[index].sal.health_all);
		printf("\n%-56s %-30.2lf","House Allowances",E[index].sal.house_all);
		printf("\n%-56s %-30.2lf","Travelling Allowances",E[index].sal.travelling_all);
	}
	printf("\n%-56s %-30.2lf","Overtime Pay",E[index].overtime_pay);
	printf("\n----------------------------------------------------------------------");
	printf("\n%-56s %-30.2lf","Gross Salary",E[index].gross);
	printf("\n----------------------------------------------------------------------");
	printf("\n%-56s %-30s","Deductions","Amount");
	printf("\n----------------------------------------------------------------------");
	printf("\n%-56s %-30.2lf","Tax Deducted",E[index].tax);
	printf("\n%-56s %-30.2lf","Loan Deducted",E[index].loan_ded);
	printf("\n%-56s %-30.2lf","Other Deductions",E[index].ded.other_deductions);
	printf("\n----------------------------------------------------------------------");
	printf("\n%-56s %-30.2lf","Total Deductions",E[index].tax + E[index].loan_ded + E[index].ded.other_deductions);
	printf("\n----------------------------------------------------------------------");
	printf("\n%-56s %-30.2lf","Net Salary",E[index].net);
	printf("\n======================================================================\n");
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
	printf("\n========================DEPARTMENTAL REPORT=============================\n");
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
		printf("%-12s :%-30s\n","Department",dept[i]);
		printf("=========================================================================\n");
		printf("%-56s %-30.2lf","Total Gross Salary",total_gross);
		printf("\n%-56s %-30.2lf","Total Tax Deducted",total_tax);
		printf("\n%-56s %-30.2lf","Total Loan Deducted",total_loan);
		printf("\n%-56s %-30.2lf","Total Other Deductions",total_other);
		printf("\n%-56s %-30.2lf","Total Net Salary",total_net);
		printf("\n%-56s %-30d","Full-time Employees",full_time_count);
		printf("\n%-56s %-30d","Part-time Employees",part_time_count);
		printf("\n%-56s %-30d\n","Contract Employees",contract_count);
		printf("=========================================================================\n");
	}
	printf("\n========================================================================\n");
}


void menu(){//main menu
	printf("\n=====================EMPLOYEE PAYROLL SYSTEM==============================\n");
	printf("%-40s%s\n","1) Add employee"               ,"6) Search employee by id");
	printf("%-40s%s\n","2) List employees"             ,"7) Run monthly payroll");
	printf("%-40s%s\n","3) Update or delete employees" ,"8) Print payslip");
	printf("%-40s%s\n","4) Attendance and overtime"    ,"9) Departmental report");
	printf("%-40s%s\n","5) View attendance matrix"     ,"10) Save or load data from file");
	printf("0) Exit\n");
	printf("==========================================================================\n");
}

void write(employee E[],int n){
	FILE *P = NULL;
	P = fopen("EMPLOYEE.csv","w");
	if(P == NULL){
		printf("Error! Could not open EMPLOYEE.csv for writing.\n");
		return ;
	}
	int i;
	fputs("Name,Id,Department,Employee_type,Basic_salary,Health_allowances,House_allowances,Travelling_allowances,Hourly_rate,Contract_pay,Loan,Loan_installments,Other_deductions,",P);
	fprintf(P,"\n");
	for(i = 0 ;i<n; i++){
	fprintf(P,"%s,%d,%s,",E[i].name,E[i].id,E[i].department);
	if(E[i].employee_type==1) fprintf(P,"%s,%.2lf,%.2lf,%.2lf,%.2lf,%c,%c,","Full_Time",E[i].sal.basic,E[i].sal.health_all,E[i].sal.house_all,E[i].sal.travelling_all,'-','-');
	else if(E[i].employee_type == 2) fprintf(P,"%s,%c,%c,%c,%c,%.2lf,%c,","Part_Time",'-','-','-','-',E[i].sal.hourly_rate,'-');
	else fprintf(P,"%s,%c,%c,%c,%c,%c,%.2lf,","Contract",'-','-','-','-','-',E[i].sal.contract_pay);
	fprintf(P,"%.2lf,%.2lf,%.2lf,",E[i].ded.loan,E[i].ded.loan_installment,E[i].ded.other_deductions);
	
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
		line[strcspn(line,"\n")]='\0';
		if(line[0]=='\0') continue;
		if(*n >= *employee_limit){
			maintain_Memory(K,employee_limit);
			E = *K;}
		
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

int main()
{
	int employee_count=0;
	int employee_limit = 2;
	int dept_count = 0;
	char department[100][dept_length];
	employee *E=(employee*)malloc(employee_limit*sizeof(employee));
	if(E==NULL){
    printf("Memory allocation failed!\n");
    exit(1);
}
	do{
		menu();
		int choice = readInt("Enter your choice",0,11);
		switch(choice){
			case 1:
			if(employee_count >= employee_limit){
				maintain_Memory(&E,&employee_limit);}
				employee_count = addemployee(E,employee_count);
				break;
			case 2:
				employee_list(E,employee_count);
				break;
			case 3:
				upd_and_del(E,&employee_count);
				break;
			case 4:
				Att_and_OT(E,employee_count);
				break;
			case 5:
				print_attendance(E,employee_count);
				break;
			case 6:
				search_employee(E,employee_count);
				break;
			case 7:
				if(flag("Are you sure you want to run monthly payroll"))
				payroll(E,employee_count);
				break;
			case 8:
				Print_payslip(E,employee_count);
				break;
			case 9:
				Departmental_report(E,employee_count,&dept_count, department);
				break;
			case 10:{
				int C = readInt("1)Save Data  2)Load Data",1,3);
				switch(C){
					case 1:
					if(flag("Do you want to save data to File"))
						write(E,employee_count);
					break;
					case 2:
					if(flag("Do you want load data from File"))
						read(&E,&employee_count,&employee_limit);
					break;
				}
				break;}
			case 0:
				if(flag("Are you sure you want to exit")){
				printf("Exiting the program.\n");
				free(E);
				return 0;}
				break;
			default:
				printf("Invalid choice. Please try again.\n");
		}
	} while(1);

}
