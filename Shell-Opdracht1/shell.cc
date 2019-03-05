#include "shell.hh"
#include <sys/wait.h>

using std::cout;
using std::endl;
using std::cin;
using std::getline;
using std::string;

int main(){
	string input;
	//HIER WERK IK MET DE SYSCALL
	int fdprompt = syscall(SYS_open,"prompt.txt",O_RDONLY); 	//file decriptor met syscall open om prompt.txt te openen in readonly
	if(fdprompt < 0){ 						//als er een error is dan is fdprompt -1
		perror("c1"); 						//geeft de error code in STDERR
		exit(1); 						//sluit functie
	}
	printf("Opened fileprompt = %d\n", fdprompt);

	char *pointerbuffer = (char *) malloc(100); 			//pointer naar geheugenlocatie voor de buffer van prompt allokeerd memory
	int szprompt = syscall(SYS_read,fdprompt, pointerbuffer,18); 	//read de file met een 18byte grootte die we in de buffer zetten
	pointerbuffer[szprompt] = '\0'; 				//zorgt dat we de bytes kunnen lezen???


	if(syscall(SYS_close,fdprompt) < 0){ 					//voor een foutmelding bij het closen van prompt.txt
		perror("close prompt");
		exit(1);
	}
	printf("Closed fileprompt\n");
	string prompt = pointerbuffer;
	free(pointerbuffer); //zorgt dat de gealokkeerde memory weer vrij wordt gegeven

  while(true){
	cout << "__________SHELL__________" << endl;
	cout << prompt << endl;         		// Print het prompt
	getline(cin, input);         			// Lees een regel
	if (input == "new_file") new_file();     	// Kies de functie
	else if (input == "ls") list();        		// op basis van
	else if (input == "src") src();        		// de invoer
	else if (input == "find") find();
	else if (input == "python") python();
	else if (input == "exit") return 0;
	else if (input == "quit") return 0;
	else if (input == "error") return 1;
	if (std::cin.eof()) return 0; 			// EOF is een exit
	}
 }

void new_file(){ // ToDo: Implementeer volgens specificatie.
	cout <<"_____NEW FILE_____"<<endl;
	string filename;
	string text;
	string endtext = "<EOF>";
	cout << "Give your filename:";
	getline(cin,filename);
	filename += ".txt";
	cout << "Give the text, end your text with <EOF>" << endl;
	getline(cin,text);
	//cout << filename << endl;
	//cout << text << endl;

	char* pointerfd = (char*)filename.c_str();
	int fd_new_file = creat(pointerfd,0777);
	open(pointerfd,O_APPEND);
	printf("Opened file %d\n", fd_new_file);

	//hier moet de text worden geschreven naar de file
	std::size_t found = text.find(endtext);
	if(found != string::npos){
		cout << "<EOF> is gevonden op index : " << found << endl;
	}

	while(found == string::npos){
		cout << "The given text doesn't include <EOF> please add it to the end of your text" << endl;
		cout <<"Give the text, end your text with <EOF>" << endl;
		getline(cin,text);
		found = text.find(endtext);
	}

	text.erase(found, string::npos); 		//verwijdert de <EOF> uit de string
	std::size_t toindex = text.size();		//de grootte van de write
	const char *textpointer = text.c_str(); 	//maakt van de string een plek in het geheugen waar we naartoe kunnen verwijzen in write

	int szwrite = write(fd_new_file,textpointer,toindex);

	if(close(fd_new_file) < 0){
		perror("close new file");
		exit(1);
	}
	printf("Closed file\n");
}

void list(){
	cout << "______LS_____" << endl;
	int pid = fork(); //create de fork

	if(pid == 0){
		// Create the child
		char *list_arguments[] = {"/bin/ls", "-la", NULL};
		execv(list_arguments[0],list_arguments);
	} else{
		// Waiting for the anti-vax to do the work
		int exit_status;
		wait(&exit_status);
	}
}

void find(){
	cout << "_____FIND_____" << endl;
	cout << "What do you want to grep?" << endl;
	string grepinput;
	getline(cin, grepinput);
	pid_t pid;
	int statusfork;
	int pipefd[2];
	int piperet = pipe(pipefd);

	if(piperet == -1){
		printf("Pipe failed/n");
		exit(1);
	}

	pid = fork();
	if(pid == 0){
		cout << "Ben in de eerste fork"<<endl;
		char *list_arguments[] = {(char*) "find",(char*) "./",(char*)NULL};
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		execv("/usr/bin/find", list_arguments);
	}
	pid = fork();
	if(pid == 0){
		cout << "Ben in de tweede fork"<< endl;
		char *list_arguments[] = {(char*)"grep",(char*) grepinput.c_str(),(char*)NULL};
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		execv("/bin/grep", list_arguments);
	}
	close(pipefd[0]);
	close(pipefd[1]);

	waitpid(-1,&statusfork,0);
	waitpid(-1,&statusfork,0);
}

void python(){
	cout << "____PYTHON_____" << endl;
	int pid = fork(); //create the fork

	if(pid==0){
		//create the child
		char *list_arguments[] = {"/usr/bin/python", NULL};
		execv(list_arguments[0],list_arguments);
	} else{
		//waiting for the anti-vak to do the work
		int exit_status;
		wait(&exit_status);
	}
}

void src(){ 								// Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
	int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); 	// Gebruik de SYS_open call om een bestand te openen.
  char byte[1];                                           		// 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
  while(syscall(SYS_read, fd, byte, 1))                   		// Blijf SYS_read herhalen tot het bestand geheel gelezen is,
std::cout << byte; 							// zet de gelezen byte in "byte" zodat deze geschreven kan worden.+++-
} 
