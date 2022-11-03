int STACKS(char *t);
struct dataStack;
struct symStack;
void InitDataStack(struct dataStack* Stacknum);
void InitSymStack(struct symStack* Stacksym);
void Popdata(struct dataStack* Stacknum, int num);
void Popsym(struct symStack* Stacksym, char sym);
int Pushdata(struct dataStack* Stacknum);
char Pushsym(struct symStack* Stacksym);
int loaddata(struct dataStack* Stacknum);
char loadsym(struct symStack* Stacksym);
int sympriority(char ch);
int math(int x,int y, char ch);


#define max 100
#define Pi 3.1415926
