#include "Code.h"

Code::Code()
{
}

Code::~Code()
{
    //dtor
}
char* Code::encode(Packet *pac){
    char *c;
    int id = pac->id;

    //Declaracion de cada struct y de ahí accede a los campos de el y de los que hereda*************
    InitialToken *orangeI;
    Token *orangePac;
    BChunk *blueC;
    BHello *blueH;
    BExist *blueE;
    BComplete *blueCom;
    BGet *blueG;
    BLocalize *blueL;
    BDelete *blueD;
    Assignment *blueOrA;
    Connect *blueOrC;
    Go *blueOrG;
    GChunk *greenC;
    GExist *greenE;
    GComplete *greenCom;
    GDelete *greenD;
    GLocalize *greenL;
    GKill *greenK;
    //*****************************

    switch(id){
    case static_cast<int>(ID::INITIAL_TOKEN):
         c = new char[7];
         memcpy(&c,&orangeI->id,sizeof(orangeI->id));
         memcpy(&c+sizeof(orangeI->id),&orangeI->ip,sizeof(orangeI->ip));
         memcpy(c+sizeof(orangeI->id)+sizeof(orangeI->ip),&orangeI->boolean,sizeof(orangeI->boolean));
        break;
    case static_cast<int>(ID::TOKEN):
         c = new char[15];
         memcpy(c,&pac->id,sizeof(pac->id));
         memcpy(c+sizeof(orangePac->id),&orangePac->ip,sizeof(orangePac->ip));
         memcpy(c+sizeof(orangePac->id)+sizeof(orangePac->ip),&orangePac->boolean,sizeof(orangePac->boolean));
         memcpy(c+sizeof(orangePac->id)+sizeof(orangePac->ip)+sizeof(orangePac->boolean),&orangePac->node,sizeof(orangePac->node));
         memcpy(c+sizeof(orangePac->id)+sizeof(orangePac->ip)+sizeof(orangePac->boolean)+sizeof(orangePac->node),&orangePac->assignedIp,sizeof(orangePac->assignedIp));
         memcpy(c+sizeof(orangePac->id)+sizeof(orangePac->ip)+sizeof(orangePac->boolean)+sizeof(orangePac->node)+sizeof(orangePac->assignedIp),&orangePac->assignedPort,sizeof(orangePac->assignedPort));
        break;
    case static_cast<int>(ID::BCHUNK):
         c = new char[10262];
         memcpy(c,&blueC->id,sizeof(blueC->id));
         memcpy(c+sizeof(blueC->id),&blueC->name[10],sizeof(blueC->name[10]));
         memcpy(c+sizeof(blueC->id)+sizeof(blueC->name[10]),&blueC->totalChunks,sizeof(blueC->totalChunks));
         memcpy(c+sizeof(blueC->id)+sizeof(blueC->name[10])+sizeof(blueC->totalChunks),&blueC->chunkNumber,sizeof(blueC->chunkNumber));
         memcpy(c+sizeof(blueC->id)+sizeof(blueC->name[10])+sizeof(blueC->totalChunks)+sizeof(blueC->chunkNumber),&blueC->data[10240],sizeof(blueC->data[10240]));
         memcpy(c+sizeof(blueC->id)+sizeof(blueC->name[10])+sizeof(blueC->totalChunks)+sizeof(blueC->chunkNumber)+sizeof(blueC->data[10240]),&blueC->hops,sizeof(blueC->hops));
        break;
    case static_cast<int>(ID::BHELLO):
         c = new char[14];
         memcpy(c,&blueH->id,sizeof(blueH->id));
         memcpy(c+sizeof(blueH->id),&blueH->name[10],sizeof(blueH->name[10]));
         memcpy(c+sizeof(blueH->id)+sizeof(blueH->name[10]),&blueH->node,sizeof(blueH->node));
    break;
    case static_cast<int>(ID::BEXIST):
         c = new char[14];
         memcpy(c,&blueE->id,sizeof(blueE->id));
         memcpy(c+sizeof(blueE->id),&blueE->name[10],sizeof(blueE->name[10]));
         memcpy(c+sizeof(blueE->id)+sizeof(blueE->name[10]),&blueE->question,sizeof(blueE->question));
         memcpy(c+sizeof(blueE->id)+sizeof(blueE->name[10])+sizeof(blueE->question),&blueE->exist,sizeof(blueE->exist));
    break;
    case static_cast<int>(ID::BCOMPLETE):
         c = new char[21];
         memcpy(c,&blueCom->id,sizeof(blueCom->id));
         memcpy(c+sizeof(blueCom->id),&blueCom->name[10],sizeof(blueCom->name[10]));
         memcpy(c+sizeof(blueCom->id)+sizeof(blueCom->name[10]),&blueCom->totalChunks,sizeof(blueCom->totalChunks));
         memcpy(c+sizeof(blueCom->id)+sizeof(blueCom->name[10])+sizeof(blueCom->totalChunks),&blueCom->question,sizeof(blueCom->question));
         memcpy(c+sizeof(blueCom->id)+sizeof(blueCom->name[10])+sizeof(blueCom->totalChunks)+sizeof(blueCom->question),&blueCom->chunkNumber,sizeof(blueCom->chunkNumber));
    break;
    case static_cast<int>(ID::BGET):
         c = new char[10261];
         memcpy(c,&blueG->id,sizeof(blueG->id));
         memcpy(c+sizeof(blueG->id),&blueG->name[10],sizeof(blueG->name[10]));
         memcpy(c+sizeof(blueG->id)+sizeof(blueG->name[10]),&blueG->totalChunks,sizeof(blueG->totalChunks));
         memcpy(c+sizeof(blueG->id)+sizeof(blueG->name[10])+sizeof(blueG->totalChunks),&blueG->question,sizeof(blueG->question));
         memcpy(c+sizeof(blueG->id)+sizeof(blueG->name[10])+sizeof(blueG->totalChunks)+sizeof(blueG->question)+sizeof(blueG->question),&blueG->chunkNumber,sizeof(blueG->chunkNumber));
         memcpy(c+sizeof(blueG->id)+sizeof(blueG->name[10])+sizeof(blueG->totalChunks)+sizeof(blueG->question)+sizeof(blueG->question)+sizeof(blueG->chunkNumber),&blueG->chunk[10240],sizeof(blueG->chunk[10240]));
    break;
    case static_cast<int>(ID::BLOCALIZE):
         c = new char[23];
         memcpy(c,&blueL->id,sizeof(blueL->id));
         memcpy(c+sizeof(blueL->id),&blueL->name[10],sizeof(blueL->name[10]));
         memcpy(c+sizeof(blueL->id)+sizeof(blueL->name[10]),&blueL->question,sizeof(blueL->question));
         memcpy(c+sizeof(blueL->id)+sizeof(blueL->name[10])+sizeof(blueL->question),&blueL->ip,sizeof(blueL->ip));
         memcpy(c+sizeof(blueL->id)+sizeof(blueL->name[10])+sizeof(blueL->question)+sizeof(blueL->ip),&blueL->port,sizeof(blueL->port));
         memcpy(c+sizeof(blueL->id)+sizeof(blueL->name[10])+sizeof(blueL->question)+sizeof(blueL->ip)+sizeof(blueL->port),&blueL->chunkNumber,sizeof(blueL->chunkNumber));

    break;
    case static_cast<int>(ID::BDELETE):
         c = new char[21];
         memcpy(c,&blueD->id,sizeof(blueD->id));
         memcpy(c+sizeof(blueD->id),&blueD->name[10],sizeof(blueD->name[10]));
    break;
    case static_cast<int>(ID::ASSIGNMENT):
         c = new char[12];
         memcpy(c,&blueOrA->id,sizeof(blueOrA->id));
         memcpy(c+sizeof(blueOrA->id),&blueOrA->nodeID,sizeof(blueOrA->nodeID));
         memcpy(c+sizeof(blueOrA->id)+sizeof(blueOrA->nodeID),&blueOrA->totalNeighbors,sizeof(blueOrA->totalNeighbors));
         memcpy(c+sizeof(blueOrA->id)+sizeof(blueOrA->nodeID)+sizeof(blueOrA->totalNeighbors),&blueOrA->ip,sizeof(blueOrA->ip));
         memcpy(c+sizeof(blueOrA->id)+sizeof(blueOrA->nodeID)+sizeof(blueOrA->totalNeighbors)+sizeof(blueOrA->ip),&blueOrA->port,sizeof(blueOrA->port));
    break;
    case static_cast<int>(ID::CONNECT):
         c = new char[2];
         memcpy(c,&blueOrC->id,sizeof(blueOrC->id));
    break;
    case static_cast<int>(ID::GO):
         c = new char[2];
         memcpy(c,&blueOrG->id,sizeof(blueOrG->id));
    break;
    case static_cast<int>(ID::GCHUNK):
         c = new char[10260];
         memcpy(c,&greenC->id,sizeof(greenC->id));
         memcpy(c+sizeof(greenC->id),&greenC->name[10],sizeof(greenC->name[10]));
         memcpy(c+sizeof(greenC->id)+sizeof(greenC->name[10]),&greenC->totalChunks,sizeof(greenC->totalChunks));
         memcpy(c+sizeof(greenC->id)+sizeof(greenC->name[10])+sizeof(greenC->totalChunks),&greenC->data[10240],sizeof(greenC->data[10240]));
    break;
    case static_cast<int>(ID::GEXIST):
         c = new char[13];
         memcpy(c,&greenE->id,sizeof(greenE->id));
         memcpy(c+sizeof(greenE->id),&greenE->name[10],sizeof(greenE->name[10]));
         memcpy(c+sizeof(greenE->id)+sizeof(greenE->name[10]),&greenE->exist,sizeof(greenE->exist));
    break;
    case static_cast<int>(ID::GCOMPLETE):
         c = new char[13];
         memcpy(c,&greenCom->id,sizeof(greenCom->id));
         memcpy(c+sizeof(greenCom->id),&greenCom->name[10],sizeof(greenCom->name[10]));
         memcpy(c+sizeof(greenCom->id)+sizeof(greenCom->name[10]),&greenCom->complete,sizeof(greenCom->complete));
    break;
    case static_cast<int>(ID::GDELETE):
         c = new char[12];
         memcpy(c,&greenD->id,sizeof(greenD->id));
         memcpy(c+sizeof(greenD->id),&greenD->name[10],sizeof(greenD->name[10]));
    break;
    case static_cast<int>(ID::GLOCALIZE):
         c = new char[23];
         memcpy(c,&greenL->id,sizeof(greenL->id));
         memcpy(c+sizeof(greenL->id),&greenL->name[10],sizeof(greenL->name[10]));
         memcpy(c+sizeof(greenL->id)+sizeof(greenL->name[10]),&greenL->question,sizeof(greenL->question));
         memcpy(c+sizeof(greenL->id)+sizeof(greenL->name[10])+sizeof(greenL->question),&greenL->ip,sizeof(greenL->ip));
         memcpy(c+sizeof(greenL->id)+sizeof(greenL->name[10])+sizeof(greenL->question)+sizeof(greenL->ip),&greenL->port,sizeof(greenL->port));
         memcpy(c+sizeof(greenL->id)+sizeof(greenL->name[10])+sizeof(greenL->question)+sizeof(greenL->ip)+sizeof(greenL->port),&greenL->chunkNumber,sizeof(greenL->chunkNumber));
    break;
    case static_cast<int>(ID::GKILL):
         c = new char[14];
         memcpy(c,&greenK->id,sizeof(greenK->id));
         memcpy(c+sizeof(greenK->id),&greenK->name[10],sizeof(greenK->name[10]));
         memcpy(c+sizeof(greenK->id)+sizeof(greenK->name[10]),&greenK->node,sizeof(greenK->node));
    break;

    }

	return c;
}

Packet* Code::decode(char *c){
	Packet *pac;
	  //Declaracion de cada struct y de ahí accede a los campos de el y de los que hereda*************
    InitialToken *orangeI;
    Token *orangePac;
    BChunk *blueC;
    BHello *blueH;
    BExist *blueE;
    BComplete *blueCom;
    BGet *blueG;
    BLocalize *blueL;
    BDelete *blueD;
    Assignment *blueOrA;
    Connect *blueOrC;
    Go *blueOrG;
    GChunk *greenC;
    GExist *greenE;
    GComplete *greenCom;
    GDelete *greenD;
    GLocalize *greenL;
    GKill *greenK;
    //*****************************
	int id;
	memcpy(&id,&c,sizeof(pac->id));
	switch(id){
		case static_cast<int>(ID::INITIAL_TOKEN):
		   pac=(OrangePacket*) calloc  (3,sizeof(OrangePacket));//********************************************************
		   memcpy(&orangeI->id,&c,sizeof(orangeI->id));
		   memcpy(&orangeI->ip,&c+sizeof(orangeI->id),sizeof(orangeI->ip));
		   memcpy(&orangeI->boolean,&c+sizeof(orangeI->id)+sizeof(orangeI->ip),sizeof(orangeI->boolean));
		   return orangeI;
		break;
		case static_cast<int>(ID::TOKEN):
           pac=(OrangePacket*) calloc  (6,sizeof(OrangePacket));
		   memcpy(&orangePac->id,&c,sizeof(orangePac->id));
		   memcpy(&orangePac->ip,&c+sizeof(orangePac->id),sizeof(orangePac->ip));
		   memcpy(&orangePac->node,&c+sizeof(orangePac->id)+sizeof(orangePac->ip),sizeof(orangePac->boolean));
		   memcpy(&orangePac->node,&c+sizeof(orangePac->id)+sizeof(orangePac->ip)+sizeof(orangePac->boolean),sizeof(orangePac->node));
		   memcpy(&orangePac->assignedIp,&c+sizeof(orangePac->id)+sizeof(orangePac->ip)+sizeof(orangePac->boolean)+sizeof(orangePac->node),sizeof(orangePac->assignedIp));
		   memcpy(&orangePac->assignedPort,&c+sizeof(orangePac->id)+sizeof(orangePac->ip)+sizeof(orangePac->boolean)+sizeof(orangePac->node)+sizeof(orangePac->assignedIp),sizeof(orangePac->assignedPort));
           return orangePac;
		break;
		 case static_cast<int>(ID::BCHUNK):
         pac=(BluePacket*) calloc  (6,sizeof(BluePacket));
         memcpy(&blueC->id,&c,sizeof(blueC->id));
         memcpy(&blueC->name[10],&c+sizeof(blueC->id),sizeof(blueC->name[10]));
         memcpy(&blueC->totalChunks,&c+sizeof(blueC->id)+sizeof(blueC->name[10]),sizeof(blueC->totalChunks));
         memcpy(&blueC->chunkNumber,&c+sizeof(blueC->id)+sizeof(blueC->name[10])+sizeof(blueC->totalChunks),sizeof(blueC->chunkNumber));
         memcpy(&blueC->data[10240],&c+sizeof(blueC->id)+sizeof(blueC->name[10])+sizeof(blueC->totalChunks)+sizeof(blueC->chunkNumber),sizeof(blueC->data[10240]));
         memcpy(&blueC->hops,&c+sizeof(blueC->id)+sizeof(blueC->name[10])+sizeof(blueC->totalChunks)+sizeof(blueC->chunkNumber)+sizeof(blueC->data[10240]),sizeof(blueC->hops));
         return blueC;
        break;
    case static_cast<int>(ID::BHELLO):
         pac=(BluePacket*) calloc  (3,sizeof(BluePacket));
         memcpy(&blueH->id,&c,sizeof(blueH->id));
         memcpy(&blueH->name[10],&c+sizeof(blueH->id),sizeof(blueH->name[10]));
         memcpy(&blueH->node,&c+sizeof(blueH->id)+sizeof(blueH->name[10]),sizeof(blueH->node));
         return blueH;
    break;
    case static_cast<int>(ID::BEXIST):
         pac=(BluePacket*) calloc  (4,sizeof(BluePacket));
         memcpy(&blueE->id,&c,sizeof(blueE->id));
         memcpy(&blueE->name[10],&c+sizeof(blueE->id),sizeof(blueE->name[10]));
         memcpy(&blueE->question,&c+sizeof(blueE->id)+sizeof(blueE->name[10]),sizeof(blueE->question));
         memcpy(&blueE->exist,&c+sizeof(blueE->id)+sizeof(blueE->name[10])+sizeof(blueE->question),sizeof(blueE->exist));
         return blueE;
    break;
    case static_cast<int>(ID::BCOMPLETE):
         pac=(BluePacket*) calloc  (5,sizeof(BluePacket));
         memcpy(&blueCom->id,&c,sizeof(blueCom->id));
         memcpy(&blueCom->name[10],&c+sizeof(blueCom->id),sizeof(blueCom->name[10]));
         memcpy(&blueCom->totalChunks,&c+sizeof(blueCom->id)+sizeof(blueCom->name[10]),sizeof(blueCom->totalChunks));
         memcpy(&blueCom->question,&c+sizeof(blueCom->id)+sizeof(blueCom->name[10])+sizeof(blueCom->totalChunks),sizeof(blueCom->question));
         memcpy(&blueCom->chunkNumber,&c+sizeof(blueCom->id)+sizeof(blueCom->name[10])+sizeof(blueCom->totalChunks)+sizeof(blueCom->question),sizeof(blueCom->chunkNumber));
         return blueCom;
    break;
    case static_cast<int>(ID::BGET):
         pac=(BluePacket*) calloc  (6,sizeof(BluePacket));
         memcpy(&blueG->id,&c,sizeof(blueG->id));
         memcpy(&blueG->name[10],&c+sizeof(blueG->id),sizeof(blueG->name[10]));
         memcpy(&blueG->totalChunks,&c+sizeof(blueG->id)+sizeof(blueG->name[10]),sizeof(blueG->totalChunks));
         memcpy(&blueG->question,&c+sizeof(blueG->id)+sizeof(blueG->name[10])+sizeof(blueG->totalChunks),sizeof(blueG->question));
         memcpy(&blueG->chunkNumber,&c+sizeof(blueG->id)+sizeof(blueG->name[10])+sizeof(blueG->totalChunks)+sizeof(blueG->question)+sizeof(blueG->question),sizeof(blueG->chunkNumber));
         memcpy(&blueG->chunk[10240],&c+sizeof(blueG->id)+sizeof(blueG->name[10])+sizeof(blueG->totalChunks)+sizeof(blueG->question)+sizeof(blueG->question)+sizeof(blueG->chunkNumber),sizeof(blueG->chunk[10240]));
         return blueG;
    break;
    case static_cast<int>(ID::BLOCALIZE):
         pac=(BluePacket*) calloc  (6,sizeof(BluePacket));
         memcpy(&blueL->id,&c,sizeof(blueL->id));
         memcpy(&blueL->name[10],&c+sizeof(blueL->id),sizeof(blueL->name[10]));
         memcpy(&blueL->question,&c+sizeof(blueL->id)+sizeof(blueL->name[10]),sizeof(blueL->question));
         memcpy(&blueL->ip,&c+sizeof(blueL->id)+sizeof(blueL->name[10])+sizeof(blueL->question),sizeof(blueL->ip));
         memcpy(&blueL->port,&c+sizeof(blueL->id)+sizeof(blueL->name[10])+sizeof(blueL->question)+sizeof(blueL->ip),sizeof(blueL->port));
         memcpy(&blueL->chunkNumber,c+sizeof(blueL->id)+sizeof(blueL->name[10])+sizeof(blueL->question)+sizeof(blueL->ip)+sizeof(blueL->port),sizeof(blueL->chunkNumber));
         return blueL;
    break;
    case static_cast<int>(ID::BDELETE):
         pac=(BluePacket*) calloc  (2,sizeof(BluePacket));
         memcpy(&blueD->id,&c,sizeof(blueD->id));
         memcpy(&blueD->name[10],&c+sizeof(blueD->id),sizeof(blueD->name[10]));
         return blueD;
    break;
    case static_cast<int>(ID::ASSIGNMENT):
         pac=(BlueOrange*) calloc  (5,sizeof(BlueOrange));
         memcpy(&blueOrA->id,&c,sizeof(blueOrA->id));
         memcpy(&blueOrA->nodeID,&c+sizeof(blueOrA->id),sizeof(blueOrA->nodeID));
         memcpy(&blueOrA->totalNeighbors,&c+sizeof(blueOrA->id)+sizeof(blueOrA->nodeID),sizeof(blueOrA->totalNeighbors));
         memcpy(&blueOrA->ip,&c+sizeof(blueOrA->id)+sizeof(blueOrA->nodeID)+sizeof(blueOrA->totalNeighbors),sizeof(blueOrA->ip));
         memcpy(&blueOrA->port,&c+sizeof(blueOrA->id)+sizeof(blueOrA->nodeID)+sizeof(blueOrA->totalNeighbors)+sizeof(blueOrA->ip),sizeof(blueOrA->port));
         return blueOrA;
    break;
    case static_cast<int>(ID::CONNECT):
         pac=(BlueOrange*) calloc  (1,sizeof(BlueOrange));
         memcpy(&blueOrC->id,&c,sizeof(blueOrC->id));
         return blueOrC;
    break;
    case static_cast<int>(ID::GO):
         pac=(BlueOrange*) calloc  (1,sizeof(BlueOrange));
         memcpy(&blueOrG->id,&c,sizeof(blueOrG->id));
    break;
    case static_cast<int>(ID::GCHUNK):
         pac=(GreenBlue*) calloc  (4,sizeof(GreenBlue));
         memcpy(&greenC->id,&c,sizeof(greenC->id));
         memcpy(&greenC->name[10],c+sizeof(greenC->id),sizeof(greenC->name[10]));
         memcpy(&greenC->totalChunks,&c+sizeof(greenC->id)+sizeof(greenC->name[10]),sizeof(greenC->totalChunks));
         memcpy(&greenC->data[10240],&c+sizeof(greenC->id)+sizeof(greenC->name[10])+sizeof(greenC->totalChunks),sizeof(greenC->data[10240]));
         return greenC;
    break;
    case static_cast<int>(ID::GEXIST):
         pac=(GreenBlue*) calloc  (3,sizeof(GreenBlue));
         memcpy(&greenE->id,&c,sizeof(greenE->id));
         memcpy(&greenE->name[10],&c+sizeof(greenE->id),sizeof(greenE->name[10]));
         memcpy(&greenE->exist,&c+sizeof(greenE->id)+sizeof(greenE->name[10]),sizeof(greenE->exist));
         return greenE;
    break;
    case static_cast<int>(ID::GCOMPLETE):
         pac=(GreenBlue*) calloc  (3,sizeof(GreenBlue));
         memcpy(&greenCom->id,&c,sizeof(greenCom->id));
         memcpy(&greenCom->name[10],&c+sizeof(greenCom->id),sizeof(greenCom->name[10]));
         memcpy(&greenCom->complete,&c+sizeof(greenCom->id)+sizeof(greenCom->name[10]),sizeof(greenCom->complete));
         return greenCom;
    break;
    case static_cast<int>(ID::GDELETE):
         pac=(GreenBlue*) calloc  (2,sizeof(GreenBlue));
         memcpy(&greenD->id,&c,sizeof(greenD->id));
         memcpy(&greenD->name[10],&c+sizeof(greenD->id),sizeof(greenD->name[10]));
         return greenD;
    break;
    case static_cast<int>(ID::GLOCALIZE):
         pac=(GreenBlue*) calloc  (6,sizeof(GreenBlue));
         memcpy(&greenL->id,&c,sizeof(greenL->id));
         memcpy(&greenL->name[10],&c+sizeof(greenL->id),sizeof(greenL->name[10]));
         memcpy(&greenL->question,&c+sizeof(greenL->id)+sizeof(greenL->name[10]),sizeof(greenL->question));
         memcpy(&greenL->ip,&c+sizeof(greenL->id)+sizeof(greenL->name[10])+sizeof(greenL->question),sizeof(greenL->ip));
         memcpy(&greenL->port,&c+sizeof(greenL->id)+sizeof(greenL->name[10])+sizeof(greenL->question)+sizeof(greenL->ip),sizeof(greenL->port));
         memcpy(&greenL->chunkNumber,&c+sizeof(greenL->id)+sizeof(greenL->name[10])+sizeof(greenL->question)+sizeof(greenL->ip)+sizeof(greenL->port),sizeof(greenL->chunkNumber));
         return greenL;
    break;
    case static_cast<int>(ID::GKILL):
         pac=(GreenBlue*) calloc  (3,sizeof(GreenBlue));
         memcpy(&greenK->id,&c,sizeof(greenK->id));
         memcpy(&greenK->name[10],&c+sizeof(greenK->id),sizeof(greenK->name[10]));
         memcpy(&greenK->node,&c+sizeof(greenK->id)+sizeof(greenK->name[10]),sizeof(greenK->node));
         return greenK;
    break;

    }

}


