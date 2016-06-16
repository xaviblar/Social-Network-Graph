//Start Date: 26/05/2013 18:00pm
//Finish Date:11/06/2013 23:00pm
//Includes of the necessary libraries

#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

#define M 5

//Global variables used in the application
bool Found=false;
long xr=-1;
bool PushUp=false;
int Minimum=(M-1)/2;
int Maximum=M-1;
int R1=-1;

fstream UsersFile("UsersFile.dat" ,ios::in | ios::out |ios::binary);

//stringConversion receives an array of 20 chars and return an string made of that array
string stringConversion(char array[20]){
	string Converted(array);
	return Converted;
}

//Declaration of Friendship structure (Arcs of the Graph)
struct Friendship{
	struct Friendship *NextFriend;
	string Friend;
};

//Declaration of Person structure (Vertex of the Graph)
struct Person{
	char Name[20];
	char Email[20];
	char Adress[20];
	char PhoneNumber[20];
	char WorkPlace[20];
	char Education[20];
	int ID;
	int FriendsIndex;
	int PostsIndex;
	struct Person *NextPerson;
	struct Friendship *FirstFriend;
	Person(){
		FriendsIndex=-1;
		PostsIndex=-1;
		NextPerson=NULL;
		FirstFriend=NULL;
	}
}Middle,*FirstPerson;

//Declaration of the FriendsFileStruct (structs to save in the Friends File)
struct FriendsFileStruct{
	char Friend[20];
	int NextFriend;
	FriendsFileStruct(){
		/*for(int i=0;i<20;){
			Friend[i]='\0';
		}*/
		NextFriend=-1;
	}
}regFriends;

//Declaration of the PostFile structure (structs to save in the Posts File)
struct PostsFile{
	char Post[150];
	int NextPost;
	PostsFile(){
		for(int i=0;i<150;i++){
			Post[i]='\0';
		}
		NextPost=-1;
	}
}regPosts;

//Declaration of the Btree struct (tree to control the Users File (Balanced tree))
struct Btree{
	int count;
	int branch[M];
    struct Person key[M-1];
    
    Btree(){
        for(int i=0;i<(M-1);i++){
            branch[i]=-1;
            key[i]=Person();
        }
        branch[M-1]=-1;
        count=0;
	}
}reg;

//Declaration of the BinaryTree (struct to save users in the binary tree file)
struct BinaryTree{
	struct Person User;
	long left;
	long right;
	BinaryTree(){
		left=0;
		right=0;
	}
}BinaryReg;

//Method to verify if a person already exists in the database
bool PersonExists(string Name){
	struct Person *temp=FirstPerson;
	while (temp!=NULL)
    {
        if (stringConversion(temp->Name)==Name)
        {
            return true;
        }
        temp=temp->NextPerson;
    }
    return false;
}

//Method to load the graph from the files where all the info is saved (Users file and friends file)
void LoadGraph(){
	fstream LoadFile("UsersFile.dat",ios::in | ios::out |ios::binary);
	fstream LoadFriends("FriendsFile.dat",ios::in | ios::out |ios::binary);
	LoadFile.seekp(0,ios::beg);
	while(!LoadFile.eof()){
		LoadFile.read(reinterpret_cast<char *> (&reg), sizeof(reg));
		for (int x=0;x<reg.count;x++){
			if (PersonExists(stringConversion(reg.key[x].Name))==false){
				struct Person *np=new Person();
				for (int i=0;i<20;i++){
					np->Name[i]=reg.key[x].Name[i];
				}
				for (int i=0;i<20;i++){
					np->Adress[i]=reg.key[x].Adress[i];
				}
				for (int i=0;i<20;i++){
					np->Education[i]=reg.key[x].Education[i];
				}
				for (int i=0;i<20;i++){
					np->Email[i]=reg.key[x].Email[i];
				}
				for (int i=0;i<20;i++){
					np->PhoneNumber[i]=reg.key[x].PhoneNumber[i];
				}
				for (int i=0;i<20;i++){
					np->WorkPlace[i]=reg.key[x].WorkPlace[i];
				}
				np->PostsIndex=reg.key[x].PostsIndex;
				np->FriendsIndex=reg.key[x].FriendsIndex;
				if (FirstPerson==NULL)
			    {
			        FirstPerson=np;
			    }
			    else
			    {
			        np->NextPerson=FirstPerson;
			        FirstPerson=np;
			    }
			    int FriendI=np->FriendsIndex;
			    while (FriendI!=-1){
			    	LoadFriends.seekg(reg.key[x].FriendsIndex*sizeof(regFriends),ios::beg);
			    	LoadFriends.read(reinterpret_cast<char *> (&regFriends),sizeof(regFriends));
			    	struct Friendship *nf=new Friendship();
				    nf->Friend=stringConversion(regFriends.Friend);
				    if (np->FirstFriend!=NULL){
				    	nf->NextFriend=np->FirstFriend;
				    	np->FirstFriend=nf;
				    }
				    else{
				    	np->FirstFriend=nf;
				    }
				    FriendI=regFriends.NextFriend;
				}

	    	}
		}
	}
	LoadFile.close();
	LoadFriends.close();
}

//Method to write a register in the file of the Btree
void writeBtree(long R,struct Btree reg){
    UsersFile.seekp(R*sizeof(reg),ios::beg);
    UsersFile.write(reinterpret_cast<char *> (&reg),sizeof(reg));
    UsersFile.flush();
}
//Method that read a register of a file and returns that register
struct Btree readBtree(long R){
    UsersFile.seekg(R*sizeof(reg),ios::beg);
    UsersFile.read(reinterpret_cast<char *> (&reg),sizeof(reg));
    return reg;
}

//Method that receives all the parameters needed for an user and asign them in the struct
struct Person PersonAttributes(struct Person Personp,string Namep, string Emailp, string Adressp,string PhoneNumberp,string WorkPlacep,string Educationp,int IDp){
	int index=Namep.length();
	for (int i=0;i<index;i++){
		Personp.Name[i]=Namep[i];
	}
	for (int u=1;u<=3;u++){
		Personp.Name[Namep.length()+u-1]='\0';
	}
	
	index=Emailp.length();
	for (int i=0;i<index;i++){
		Personp.Email[i]=Emailp[i];
	}
	
	index=Adressp.length();
	for (int i=0;i<index;i++){
		Personp.Adress[i]=Adressp[i];
	}
	
	index=PhoneNumberp.length();
	for (int i=0;i<index;i++){
		Personp.PhoneNumber[i]=PhoneNumberp[i];
	}
	
	index=WorkPlacep.length();
	for (int i=0;i<index;i++){
		Personp.WorkPlace[i]=WorkPlacep[i];
	}
	
	index=Educationp.length();
	for (int i=0;i<index;i++){
		Personp.Education[i]=Educationp[i];
	}
	
	Personp.ID=IDp;
	Personp.FriendsIndex=-1;
	Personp.PostsIndex=-1;
	return Personp;
}
//Method that searches the branch to go for saving in the Btree file
int SearchNode(struct Person Personp){
    int j=0;
    if (strcmp(reg.key[0].Name,Personp.Name)>0){
        Found = false;
        j=0;
    }
    else{
        j = reg.count;
        while (strcmp(reg.key[j-1].Name,Personp.Name)>0 && j>1){
        	j--;
        	if (strcmp(Personp.Name,reg.key[j-1].Name)==0){
        		Found=true;
        	}
        }
    }
    return j; 
}

//Method that insert a new register in the Btree File
struct Btree InsertLeaf(struct Person mid,int k,struct Btree reg){
    for(int i=reg.count; i>= k+1; i--){    
        reg.key[i] = reg.key[i-1];
        reg.branch[i+1] = reg.branch[i];
    }
    reg.key[k] = mid;
    reg.branch[k+1] = xr;
    reg.count++;
    return reg;
}
//Method that return the index of the last element in the users file
int LastRegister(){
    int a;
    fstream Register("UsersFile.dat", ios::in | ios::out | ios::binary);
    Register.seekp(0,ios::end);
    a = Register.tellg()/sizeof(reg);
    Register.close();
    return a;
}
//Method that return the index of the last element in the binary tree file
int LastRegisterBinary(){
    long a;
    fstream Register("BinaryTree.dat", ios::in | ios::out | ios::binary);
    Register.seekg(0,ios::end);
    a = Register.tellg()/sizeof(BinaryReg)-1;
    Register.close();
    return a;
}
//Method that return the index of the last element in the posts file
int LastRegisterPosts(){
    long a;
    fstream Register("PostsFile.dat", ios::in | ios::out | ios::binary);
    Register.seekg(0,ios::end);
    a = Register.tellg()/sizeof(regPosts);
    Register.close();
    return a;
}
//Method that divides a node of the btree in longers and shorters
void DivideNode(struct Person mid,int k){
    int pos=0;
    struct Btree Biggers;
    if (k <= Minimum)
        pos = Minimum;
    else
        pos = Minimum + 1;

    for(int i= pos+1; i <= Maximum; i++){
        Biggers.key[i-pos-1] = reg.key[i-1];
        Biggers.branch[i-pos] = reg.branch[i];
    }

    Biggers.count = Maximum - pos;
    reg.count = pos;

    if (k <= Minimum){
        reg= InsertLeaf(mid, k,reg);
    }
    else{
        Biggers = InsertLeaf(mid, (k - pos),Biggers);
    }

    Middle = reg.key[reg.count-1];
    Biggers.branch[0] = reg.branch[reg.count];
    xr = LastRegister();
    long Last = LastRegister();
    writeBtree(Last, Biggers);
    reg.count--;
}

//Method that push the new node in the corresponding place in the btree File
void PushMethod(struct Person Personp, int R){
    int k=0;
    Found = false;
    if (R == -1){
        PushUp = true;
        Middle = Personp;
        xr = -1;
    }
    else{
        reg = readBtree(R);
        k = SearchNode(Personp);
        if (Found==true){
            cout << "There is already a person with that name, please choose another\n";
            PushUp = false;
        }
        else{
            PushMethod(Personp, reg.branch[k]);
            if (PushUp==true){
                if (reg.count < (M-1)){
                    PushUp = false;
                    if (R==0){
                    	reg=InsertLeaf(Middle, k,readBtree(0));
                    }
					else{
						reg=InsertLeaf(Middle,k,reg);
					}
					writeBtree(R,reg);    
                }
                else{
                    PushUp = true;
                    DivideNode(Middle,k);
                    if (R==0){
                        R =LastRegister();
                        R1 = R;
                    }
                    writeBtree(R, reg);
                }
            }
        }
    }
}
//Method that inserts a new node in the binary tree file
void InsertBinary(string Namep,string Emailp,string Adressp,string PhoneNumberp,string WorkPlacep,string Educationp,int IDp){
	struct Person np;
	np=PersonAttributes(np,Namep,Emailp,Adressp,PhoneNumberp,WorkPlacep,Educationp,IDp);
    int index=0;
    bool FirstTime;
    fstream BinaryFile("BinaryTree.dat",ios::in | ios::out |ios::binary);

	BinaryFile.seekp(0,ios::end);

    if (BinaryFile.tellg()!=0){
		FirstTime=false;
    }
    else{
    	FirstTime=true;
    }
    
    BinaryReg.User=np;
    BinaryReg.left=-1;
    BinaryReg.right=-1;
    BinaryFile.write(reinterpret_cast<char*> (&BinaryReg),sizeof(BinaryReg));
    
    if (FirstTime==true){
    	return;
    }

    while (true){
        BinaryFile.seekg(index * sizeof(BinaryReg));
        BinaryFile.read(reinterpret_cast<char*> (&BinaryReg),sizeof(BinaryReg));

        if (strcmp(BinaryReg.User.Name,np.Name)==-1){
            if (BinaryReg.right==-1){
                    BinaryReg.right=LastRegisterBinary();
                    BinaryFile.seekg(index * sizeof(BinaryReg));
                    BinaryFile.write(reinterpret_cast<char*> (&BinaryReg),sizeof(BinaryReg));
                    break;

            }
            else{
                index=BinaryReg.right;  //Vuelve al ciclo con el indice de la derecha
            }
        }
        else{
            if (BinaryReg.left==-1){
                    BinaryReg.left=LastRegisterBinary();
                    BinaryFile.seekg(index * sizeof(BinaryReg));
                    BinaryFile.write(reinterpret_cast<char*> (&BinaryReg),sizeof(BinaryReg));
                    break;

            }
            else{
                index=BinaryReg.left;  //Vuelve al ciclo con el indice de la derecha
            }
        }
    }
    BinaryFile.close();
}

//Main method of the insertion of a new person in the Btree file
void NewPerson(string Namep, string Emailp, string Adressp,string PhoneNumberp,string WorkPlacep,string Educationp,int IDp,int R){
	struct Person np;
	np=PersonAttributes(np,Namep,Emailp,Adressp,PhoneNumberp,WorkPlacep,Educationp,IDp);
    PushMethod(np,R);
    if (PushUp==true){
        reg.count = 1;
        reg.key[0] =Middle;
        reg.branch[0]=R1;
        reg.branch[1]= xr;
        writeBtree(0, reg);
    }
    cout<<"The new person has been succesfully added\n";
}
//Aux method for inserting users in the Btree file
void CreateNewUser(string Namep, string Emailp, string Adressp,string PhoneNumberp,string WorkPlacep,string Educationp,int IDp){
	
    UsersFile.seekp(0,ios::end);
    long size = UsersFile.tellg();
    if( size == 0)
    {
         NewPerson(Namep,Emailp,Adressp,PhoneNumberp,WorkPlacep,Educationp,IDp,-1);
    }
	else{
		NewPerson(Namep,Emailp,Adressp,PhoneNumberp,WorkPlacep,Educationp,IDp,0);
	}
	
	struct Person *np=new Person;
	int index=Namep.length();
	for (int i=0;i<index;i++){
		np->Name[i]=Namep[i];
	}
	
	index=Emailp.length();
	for (int i=0;i<index;i++){
		np->Email[i]=Emailp[i];
	}
	
	index=Adressp.length();
	for (int i=0;i<index;i++){
		np->Adress[i]=Adressp[i];
	}
	
	index=PhoneNumberp.length();
	for (int i=0;i<index;i++){
		np->PhoneNumber[i]=PhoneNumberp[i];
	}
	
	index=WorkPlacep.length();
	for (int i=0;i<index;i++){
		np->WorkPlace[i]=WorkPlacep[i];
	}
	
	index=Educationp.length();
	for (int i=0;i<index;i++){
		np->Education[i]=Educationp[i];
	}
	np->ID=IDp;
	np->FriendsIndex=-1;
	np->PostsIndex=-1;
	if (FirstPerson==NULL)
    {
        FirstPerson=np;
    }
    else
    {
        np->NextPerson=FirstPerson;
        FirstPerson=np;
    }
    InsertBinary(Namep,Emailp,Adressp,PhoneNumberp,WorkPlacep,Educationp,IDp);
}
//Method That prints the Btree file
void PrintBtree(){
    fstream BtreePrint("UsersFile.dat",ios::in | ios::out |ios::binary);
    BtreePrint.seekg(0);
    while (!BtreePrint.eof()){
        BtreePrint.read(reinterpret_cast<char *> (&reg), sizeof(reg));
        if(!BtreePrint.eof()){
        	for (int i=0;i<M-1;i++){
            	cout<<reg.branch [i]<<" "<<stringConversion(reg.key[i].Name)<<" ";
	        }
	        cout<<reg.branch [M-1]<<" "<<reg.count<<endl;
	    }

    }
    BtreePrint.close();
}
//Method that receives a name and return the person vertex with that name
struct Person *Search(string Name)
{
    struct Person *temp=FirstPerson;
    while (temp!=NULL)
    {
        if (stringConversion(temp->Name)==Name)
        {
            return temp;
        }
        temp=temp->NextPerson;
    }
};

//Method that link two nodes as friends mutually (in the files and in the graph)
void newFriend(string Userp,string NameFriend)
{
	struct Person *User=Search(Userp);
	struct Person *Friend=Search(NameFriend);
	
	struct Friendship *temp=User->FirstFriend;
	while(temp!=NULL){
		if (temp->Friend==NameFriend){
			cout<<"You are already friends with that person\n";
			return;
		}
		temp=temp->NextFriend;
	}
	
    struct Friendship *nfv=new Friendship();
    nfv->Friend=NameFriend;
    if (User->FirstFriend!=NULL){
    	nfv->NextFriend=User->FirstFriend;
    	User->FirstFriend=nfv;
    }
    else{
    	User->FirstFriend=nfv;
    }
    
    struct Friendship *nfv2=new Friendship();
    nfv2->Friend=Userp;
    if(Friend->FirstFriend!=NULL){
    	nfv2->NextFriend=Friend->FirstFriend;
    	Friend->FirstFriend=nfv2;
    }
    else{
    	Friend->FirstFriend=nfv2;
    }
    
    fstream NewFriend("FriendsFile.dat" ,ios::in | ios::out |ios::binary);
    fstream NewFriend2("UsersFile.dat" ,ios::in | ios::out |ios::binary);
    
    struct FriendsFileStruct nf;
    
    int index=User->FriendsIndex;
    int index2=0;
    int index3=0;
    int IPerson=0;
    if (index==-1){
    	NewFriend2.seekg(0,ios::beg);
    	while(!NewFriend2.eof()){
    		bool CurrentUser=true;
			NewFriend2.read(reinterpret_cast<char *> (&reg), sizeof(reg));
			for(int i=0;i<=reg.count;i++){
				CurrentUser=true;
				for (int y=0;y<Userp.length();y++){
					if(reg.key[i].Name[y]!=Userp[y]){
						CurrentUser=false;
						break;
					}
				}
				if (CurrentUser==true){
					IPerson=i;
					break;
				}
			}
			if (CurrentUser==true){
				break;
			}
			else{
				index3++;
			}			
		}
		NewFriend2.seekg(index3*sizeof(reg));
		NewFriend2.read(reinterpret_cast<char *> (&reg), sizeof(reg));
		NewFriend.seekp(0,ios::end); 
		reg.key[IPerson].FriendsIndex=NewFriend.tellg()/sizeof(nf);
		User->FriendsIndex=NewFriend.tellg()/sizeof(nf);
		NewFriend2.seekp(index3*sizeof(reg));
		NewFriend2.write(reinterpret_cast<char *> (&reg),sizeof(reg));
		
    }
    else{
    	while(index!=-1){
    	NewFriend.seekg(index*sizeof(nf));
    	NewFriend.read(reinterpret_cast<char*> (&nf),sizeof(nf));
    	index2=index;
    	index=nf.NextFriend;
	    }
	    NewFriend.flush();
		NewFriend.seekp(0,ios::end); 
		nf.NextFriend=NewFriend.tellg()/sizeof(nf);
	    NewFriend.seekp(index2*sizeof(nf),ios::beg);
	    NewFriend.write(reinterpret_cast<char *> (&nf),sizeof(nf));
    }
    
    for (int i=0;i<NameFriend.length();i++){
    	nf.Friend[i]=NameFriend[i];
    }
    for (int u=1;u<=3;u++){
    	nf.Friend[NameFriend.length()+u-1]=' ';
    }
    nf.NextFriend=-1;
    NewFriend.seekp(0,ios::end);
    NewFriend.write(reinterpret_cast<char *> (&nf),sizeof(nf));
    
    struct FriendsFileStruct nf2;
    index=Friend->FriendsIndex;
    index2=0;
    index3=0;
    IPerson=0;
    if (index==-1){
    	NewFriend2.seekg(0,ios::beg);
    	while(!NewFriend2.eof()){
    		bool CurrentUser=true;
			NewFriend2.read(reinterpret_cast<char *> (&reg), sizeof(reg));
			for(int i=0;i<=reg.count;i++){
				CurrentUser=true;
				for (int y=0;y<Userp.length();y++){
					if(reg.key[i].Name[y]!=Userp[y]){
						CurrentUser=false;
						break;
					}
				}
				if (CurrentUser==true){
					IPerson=i;
					break;
				}
			}
			if (CurrentUser==true){
				break;
			}
			else{
				index3++;
			}			
		}
		NewFriend2.seekg(index3*sizeof(reg));
		NewFriend2.read(reinterpret_cast<char *> (&reg), sizeof(reg));
		NewFriend.seekp(0,ios::end); 
		reg.key[IPerson].FriendsIndex=NewFriend.tellg()/sizeof(nf2);
		Friend->FriendsIndex=NewFriend.tellg()/sizeof(nf2);
		NewFriend2.seekp(index3*sizeof(reg));
		NewFriend2.write(reinterpret_cast<char *> (&reg),sizeof(reg));
		
    }
    else{
    	while(index!=-1){
    	NewFriend.seekg(index*sizeof(nf2));
    	NewFriend.read(reinterpret_cast<char*> (&nf2),sizeof(nf2));
    	index2=index;
    	index=nf2.NextFriend;
	    }
		NewFriend.seekp(0,ios::end); 
		nf2.NextFriend=NewFriend.tellg()/sizeof(nf2);
	    NewFriend.seekp(index2*sizeof(nf2),ios::beg);
	    NewFriend.write(reinterpret_cast<char *> (&nf2),sizeof(nf2));
    }
    for (int i=0;i<Userp.length();i++){
    	nf2.Friend[i]=Userp[i];
    }
    for (int u=1;u<=3;u++){
    	nf2.Friend[Userp.length()+u-1]=' ';
    }
    nf2.NextFriend=-1;
    NewFriend.seekp(0,ios::end);
    NewFriend.write(reinterpret_cast<char *> (&nf2),sizeof(nf2));
    
    cout<<"You are now friends with "<<NameFriend<<endl;  
    
    NewFriend.close();
    NewFriend2.close();
}

//Method that receives an user and a post and assign it in the list of posts of that user
void newPost(string user,string Post)
{
    fstream NewPost("PostsFile.dat" ,ios::in | ios::out |ios::binary);
    fstream NewPost2("BinaryTree.dat" ,ios::in | ios::out |ios::binary);
    struct Person *User=Search(user);
    struct PostsFile nf;
    int index=User->PostsIndex;
    int index2=0;
    int index3=0;
    cout<<index;
    if (index==-1){
    	NewPost2.seekg(0,ios::beg);
    	while(!NewPost2.eof()){
			NewPost2.read(reinterpret_cast<char *> (&BinaryReg), sizeof(BinaryReg));
			if(strcmp(User->Name,BinaryReg.User.Name)==0){
				BinaryReg.User.PostsIndex=LastRegisterPosts();
				User->PostsIndex=LastRegisterPosts();
				NewPost2.seekp(index3*sizeof(BinaryReg));
				NewPost2.write(reinterpret_cast<char *> (&BinaryReg),sizeof(BinaryReg));
			}
			index3++;	
		}
    }
    else{
    	while(index!=-1){
	    	NewPost.seekg(index*sizeof(nf));
	    	NewPost.read(reinterpret_cast<char*> (&nf),sizeof(nf));
	    	index2=index;
	    	index=nf.NextPost;
	    }
	    NewPost.flush();
		nf.NextPost=LastRegisterPosts();
	    NewPost.seekp(index2*sizeof(nf),ios::beg);
	    NewPost.write(reinterpret_cast<char *> (&nf),sizeof(nf));
    }
    
    for (int i=0;i<Post.length();i++){
    	nf.Post[i]=Post[i];
    }

    nf.NextPost=-1;
    NewPost.seekp(0,ios::end);
    NewPost.write(reinterpret_cast<char *> (&nf),sizeof(nf));
    NewPost.close();
    NewPost2.close();
}
//Method that prints the Friends file
void PrintFriendsFile(){
	fstream PrintFriends("FriendsFile.dat",ios::in | ios::out |ios::binary);
	PrintFriends.seekg(0,ios::beg);
	int cont=0;
	while(!PrintFriends.eof()){
		PrintFriends.read(reinterpret_cast<char *> (&regFriends),sizeof(regFriends));
		if(!PrintFriends.eof()){
			cout<<cont<<" "<<stringConversion(regFriends.Friend)<<" "<<regFriends.NextFriend<<endl;
		}
		cont++;
	}
	PrintFriends.close();
}
//Method that prints the posts file
void PrintPostsFile(){
	fstream PrintPosts("PostsFile.dat",ios::in | ios::out |ios::binary);
	PrintPosts.seekg(0,ios::beg);
	int cont=0;
	while(!PrintPosts.eof()){
		PrintPosts.read(reinterpret_cast<char *> (&regPosts),sizeof(regPosts));
		if(!PrintPosts.eof()){
			cout<<cont<<" "<<stringConversion(regPosts.Post)<<" "<<regPosts.NextPost<<endl;
		}
		cont++;
	}
	PrintPosts.close();
}
//Method that prints the binary tree file
void PrintBinaryTree(){
    int cont=0;
    fstream PrintBinary("BinaryTree.dat",ios::in | ios::out |ios::binary);
    PrintBinary.seekg(0);
    while(!PrintBinary.eof()){
        PrintBinary.read(reinterpret_cast<char*> (&BinaryReg),sizeof(BinaryReg));
        if(!PrintBinary.eof()){
        	cout<<cont<<"  "<<BinaryReg.left<<" "<<stringConversion(BinaryReg.User.Name)<<" "<<BinaryReg.right<<endl;
        }
        cont++;
    }
}

//Method that prints the graph (vertex and arcs)
void PrintGraph(){
	struct Person *temp=FirstPerson;
	while(temp!=NULL){
		cout<<endl<<stringConversion(temp->Name)<<" Amigos: ";
		struct Friendship *temp2=temp->FirstFriend;
		while (temp2!=NULL){
			cout<<temp2->Friend<<endl;
			temp2=temp2->NextFriend;
		}
		temp=temp->NextPerson;
	}
}

//Method to print the list of the friends of an user
void ShowFriends(string UserName){
	struct Person *User=Search(UserName);
	if(User->FriendsIndex==-1){
		cout<<"\nYou dont have any friend\n";
		return;
	}
	fstream FriendsSearch("FriendsFile.dat",ios::in | ios::out |ios::binary);
	int index=User->FriendsIndex;
	cout<<"\nFriends List: \n";
	while(index!=-1){
		FriendsSearch.seekg(User->FriendsIndex*sizeof(regFriends));
		FriendsSearch.read(reinterpret_cast<char*> (&regFriends),sizeof(regFriends));
		struct Person *Friend=Search(stringConversion(regFriends.Friend));
		cout<<stringConversion(Friend->Name)<<"\nEmail: "<<stringConversion(Friend->Email)<<"\nAdress: "<<stringConversion(Friend->Adress);
		index=regFriends.NextFriend;
	}

}

//Main function of the application (Variables and console menu here)
int main(){
	string UserName;
	int opcion;
	string NewName;
	string Email;
	string Adress;
	string PhoneNumber;
	string WorkPlace;
	string Education;
	string Post;
	int ID;
	LoadGraph();
	/*CreateNewUser("Ana","sag","asfg","asfg","asfg","asfg",64);
	CreateNewUser("Beto","sag","asfg","asfg","asfg","asfg",64);
	CreateNewUser("Carlos","sag","asfg","asfg","asfg","asfg",64);
	CreateNewUser("Diego","sag","asfg","asfg","asfg","asfg",64);
	CreateNewUser("Figo","sag","asfg","asfg","asfg","asfg",64);
	CreateNewUser("Guti","sag","asfg","asfg","asfg","asfg",64);
	CreateNewUser("Hunter","sag","asfg","asfg","asfg","asfg",64);
	CreateNewUser("Ibra","sag","asfg","asfg","asfg","asfg",64);
	CreateNewUser("Antony","sag","asfg","asfg","asfg","asfg",64);*/
	while (true){
		inicio:
		cout<<"-----------------------Welcome to the Social Network---------------------------\n";
		cout<<"Please enter your name: ";
		cin>>UserName;
		if (PersonExists(UserName)==false){
			cout<<"That person doesn't exists\n";
			system("pause");
			system("cls");
			goto inicio;
		}
		else{
			cout<<"Welcome "<<UserName<<endl;
		}
		while (true){
			MainMenu:
			cout<<"----------Main Menu----------\n";
			cout<<"Enter 1 to register a new User\n"<<"Enter 2 to see your FriendList\n";
			cout<<"Enter 3 to see the posts of your friends\n"<<"Enter 4 to add a new Friend\n";
			cout<<"Enter 5 to print the Btree File\n"<<"Enter 6 to print the BinaryTree File\n";
			cout<<"Enter 7 to print the Friends File\n"<<"Enter 8 to print the Posts File\n";
			cout<<"Enter 9 to post something\n";
			cout<<"Enter 10 to exit the program\n";
			cin>>opcion;
			if (opcion==1){
				cout<<"Please Enter the requested info (every argument is limited to 20 characters Maximum)\n";
				cout<<"Enter the name of the new user: ";
				cin>>NewName;
				if (PersonExists(NewName)==true){
					"\nThere is already a person with that name, please choose another";
					goto MainMenu;
				}
				cout<<"\nEnter the Email: ";
				cin>>Email;
				cout<<"\nEnter the Adress: ";
				cin>>Adress;
				cout<<"\nEnter the PhoneNumber: ";
				cin>>PhoneNumber;
				cout<<"\nEnter the Workplace: ";
				cin>>WorkPlace;
				cout<<"\nEnter your education place: ";
				cin>>Education;
				cout<<"\nEnter the ID (numbers): ";
				cin>>ID;
				CreateNewUser(NewName,Email,Adress,PhoneNumber,WorkPlace,Education,ID);
				system("pause");
				system("cls");
				goto MainMenu;
			}
			else if(opcion==2){
				ShowFriends(UserName);
				system("pause");
				goto MainMenu;
			}
			else if(opcion==3){
				//ShowPosts(UserName);
				system("pause");
				goto MainMenu;
			}
			else if(opcion==4){
				cout<<"\nEnter the name of the person to add: ";
				cin>>NewName;
				if(PersonExists(NewName)==false){
					cout<<"That person does not exists\n";
					system("pause");
					system("cls");
					goto MainMenu;
				}
				newFriend(UserName,NewName);
				system("pause");
				system("cls");
				goto MainMenu;
			}
			else if(opcion==5){
				PrintBtree();
				system("pause");
				system("cls");
				goto MainMenu;
			}
			else if(opcion==6){
				PrintBinaryTree();
				system("pause");
				system("cls");
				goto MainMenu;
			}
			else if(opcion==7){
				PrintFriendsFile();
				system("pause");
				system("cls");
				goto MainMenu;
			}
			else if(opcion==8){
				PrintPostsFile();
				system("pause");
				system("cls");
				goto MainMenu;
			}
			else if(opcion==9){
				getline(cin,Post);
                cout<<"\nEnter your post (150 char maximum):  ";
                getline(cin,Post);
                newPost(UserName,Post);
                system("pause");
				system("cls");
				goto MainMenu;
			}
			else if(opcion==10){
				break;
			}
		}
		break;
	}
	UsersFile.close();
	return 0;
}

