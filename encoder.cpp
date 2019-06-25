#include "encoder.h"
#include <iostream>
using namespace std;

Code::Code()
{
}

Code::~Code()
{
    //dtor
}
char* Code::encode(Packet *pac){
    char *c;
    unsigned char id = pac->id;

    switch((unsigned int) id){
    case static_cast<int>(ID::INITIAL_TOKEN):
         c = new char[6]();
         memcpy(c, &pac->id,sizeof(Packet::id));
         memcpy(c + sizeof(Packet::id),&((OrangePacket*)pac)->ip,sizeof(OrangePacket::ip));
        break;
    case static_cast<int>(ID::TOKEN_EMPTY):
         c = new char[15]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c + sizeof(Packet::id),&((OrangePacket*)pac)->ip,sizeof(OrangePacket::ip));
         memcpy(c + sizeof(Packet::id) + sizeof(OrangePacket::ip),&((Token*)pac)->boolean,sizeof(Token::boolean));
         memcpy(c + sizeof(Packet::id) + sizeof(OrangePacket::ip) + sizeof(Token::boolean),&((Token*)pac)->node,sizeof(Token::node));
         memcpy(c + sizeof(Packet::id) + sizeof(OrangePacket::ip) + sizeof(Token::boolean) + sizeof(Token::node),&((Token*)pac)->assignedIp,sizeof(Token::assignedIp));
         memcpy(c + sizeof(Packet::id) + sizeof(OrangePacket::ip) + sizeof(Token::boolean) + sizeof(Token::node)+sizeof(Token::assignedIp),&((Token*)pac)->assignedPort,sizeof(Token::assignedPort));
        break;
    case static_cast<int>(ID::BCHUNK):
         c = new char[10262]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c + sizeof(Packet::id),&((BluePacket*)pac)->name[10],sizeof(BluePacket::name[10]));
         memcpy(c + sizeof(Packet::id)+sizeof(BluePacket::name[10]),&((BChunk*)pac)->totalChunks,sizeof(BChunk::totalChunks));
         memcpy(c + sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BChunk::totalChunks),&((BChunk*)pac)->chunkNumber,sizeof(BChunk::chunkNumber));
         memcpy(c + sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BChunk::totalChunks)+sizeof(BChunk::chunkNumber),&((BChunk*)pac)->data[10240],sizeof(BChunk::data[10240]));
         memcpy(c + sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BChunk::totalChunks)+sizeof(BChunk::chunkNumber)+sizeof(BChunk::data[10240]),&((BChunk*)pac)->hops,sizeof(BChunk::hops));
        break;
    case static_cast<int>(ID::BHELLO):
         c = new char[14]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((BluePacket*)pac)->name[10],sizeof(BluePacket::name[10]));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10]),&((BHello*)pac)->node,sizeof(BHello::node));
    break;
    case static_cast<int>(ID::BEXIST):
         c = new char[14]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((BluePacket*)pac)->name[10],sizeof(BluePacket::name[10]));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10]),&((BExist*)pac)->question,sizeof(BExist::question));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BExist::question),&((BExist*)pac)->exist,sizeof(BExist::exist));
    break;
    case static_cast<int>(ID::BCOMPLETE):
         c = new char[21]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((BluePacket*)pac)->name[10],sizeof(BluePacket::name[10]));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10]),&((BComplete*)pac)->totalChunks,sizeof(BComplete::totalChunks));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BComplete::totalChunks),&((BComplete*)pac)->question,sizeof(BComplete::question));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BComplete::totalChunks)+sizeof(BComplete::question),&((BComplete*)pac)->chunkNumber,sizeof(BComplete::chunkNumber));
    break;
    case static_cast<int>(ID::BGET):
         c = new char[10261]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((BluePacket*)pac)->name[10],sizeof(BluePacket::name[10]));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10]),&((BGet*)pac)->totalChunks,sizeof(BGet::totalChunks));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BGet::totalChunks),&((BGet*)pac)->question,sizeof(BGet::question));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BGet::totalChunks)+sizeof(BGet::question),&((BGet*)pac)->chunkNumber,sizeof(BGet::chunkNumber));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BGet::totalChunks)+sizeof(BGet::question)+sizeof(BGet::chunkNumber),&((BGet*)pac)->chunk[10240],sizeof(BGet::chunk[10240]));
    break;
    case static_cast<int>(ID::BLOCALIZE):
         c = new char[23]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((BluePacket*)pac)->name[10],sizeof(BluePacket::name[10]));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10]),&((BLocalize*)pac)->question,sizeof(BLocalize::question));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BLocalize::question),&((BLocalize*)pac)->ip,sizeof(BLocalize::ip));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BLocalize::question)+sizeof(BLocalize::ip),&((BLocalize*)pac)->port,sizeof(BLocalize::port));
         memcpy(c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BLocalize::question)+sizeof(BLocalize::ip)+sizeof(BLocalize::port),&((BLocalize*)pac)->chunkNumber,sizeof(BLocalize::chunkNumber));

    break;
    case static_cast<int>(ID::BDELETE):
         c = new char[21]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((BluePacket*)pac)->name[10],sizeof(BluePacket::name[10]));
    break;
    case static_cast<int>(ID::ASSIGNMENT):
         c = new char[12]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((Assignment*)pac)->nodeID,sizeof(Assignment::nodeID));
         memcpy(c+sizeof(Packet::id)+sizeof(Assignment::nodeID),&((Assignment*)pac)->totalNeighbors,sizeof(Assignment::totalNeighbors));
         memcpy(c+sizeof(Packet::id)+sizeof(Assignment::nodeID)+sizeof(Assignment::totalNeighbors),&((Assignment*)pac)->ip,sizeof(Assignment::ip));
         memcpy(c+sizeof(Packet::id)+sizeof(Assignment::nodeID)+sizeof(Assignment::totalNeighbors)+sizeof(Assignment::ip),&((Assignment*)pac)->port,sizeof(Assignment::port));
    break;
    case static_cast<int>(ID::CONNECT):
         c = new char[2]();
         memcpy(c,&pac->id,sizeof(Packet::id));
    break;
    case static_cast<int>(ID::GO):
         c = new char[2]();
         memcpy(c,&pac->id,sizeof(Packet::id));
    break;
    case static_cast<int>(ID::GCHUNK):
         c = new char[10260]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((GreenBlue*)pac)->name[10],sizeof(GreenBlue::name[10]));
         memcpy(c+sizeof(Packet::id)+sizeof(GreenBlue::name[10]),&((GChunk*)pac)->totalChunks,sizeof(GChunk::totalChunks));
         memcpy(c+sizeof(Packet::id)+sizeof(GreenBlue::name[10])+sizeof(GChunk::totalChunks),&((GChunk*)pac)->data[10240],sizeof(GChunk::data[10240]));
    break;
    case static_cast<int>(ID::GEXIST):
         c = new char[13]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((GreenBlue*)pac)->name[10],sizeof(GreenBlue::name[10]));
         memcpy(c+sizeof(Packet::id)+sizeof(GreenBlue::name[10]),&((GExist*)pac)->exist,sizeof(GExist::exist));
    break;
    case static_cast<int>(ID::GCOMPLETE):
         c = new char[13]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((GreenBlue*)pac)->name[10],sizeof(GreenBlue::name[10]));
         memcpy(c+sizeof(Packet::id)+sizeof(GreenBlue::name[10]),&((GComplete*)pac)->complete,sizeof(GComplete::complete));
    break;
    case static_cast<int>(ID::GDELETE):
         c = new char[12]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((GreenBlue*)pac)->name[10],sizeof(GreenBlue::name[10]));
    break;
    case static_cast<int>(ID::GLOCALIZE):
         c = new char[23]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((GreenBlue*)pac)->name[10],sizeof(GreenBlue::name[10]));
         memcpy(c+sizeof(Packet::id)+sizeof(GreenBlue::name[10]),&((GLocalize*)pac)->question,sizeof(GLocalize::question));
         memcpy(c+sizeof(Packet::id)+sizeof(GreenBlue::name[10])+sizeof(GLocalize::question),&((GLocalize*)pac)->ip,sizeof(GLocalize::ip));
         memcpy(c+sizeof(Packet::id)+sizeof(GreenBlue::name[10])+sizeof(GLocalize::question)+sizeof(GLocalize::ip),&((GLocalize*)pac)->port,sizeof(GLocalize::port));
         memcpy(c+sizeof(Packet::id)+sizeof(GreenBlue::name[10])+sizeof(GLocalize::question)+sizeof(GLocalize::ip)+sizeof(GLocalize::port),&((GLocalize*)pac)->chunkNumber,sizeof(GLocalize::chunkNumber));
    break;
    case static_cast<int>(ID::GKILL):
         c = new char[14]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c+sizeof(Packet::id),&((GreenBlue*)pac)->name[10],sizeof(GreenBlue::name[10]));
         memcpy(c+sizeof(Packet::id)+sizeof(GreenBlue::name[10]),&((GKill*)pac)->node,sizeof(GKill::node));
    break;

    }
	return c;
}

Packet* Code::decode(char *c){
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
	unsigned char id = 0;
	memcpy(&id, c,sizeof(Packet::id));
	switch((unsigned int) id){
		case static_cast<int>(ID::INITIAL_TOKEN):
		   orangeI=(InitialToken*) calloc(1,sizeof(InitialToken));
		   memcpy(&orangeI->id, c,sizeof(Packet::id));
		   memcpy(&orangeI->ip,c+sizeof(Packet::id),sizeof(OrangePacket::ip));
		   return orangeI;
		break;
		case static_cast<int>(ID::TOKEN_EMPTY):
           orangePac=(Token*) calloc  (1,sizeof(Token));
		   memcpy(&orangePac->id,c,sizeof(Packet::id));
		   memcpy(&orangePac->ip,c+sizeof(Packet::id),sizeof(OrangePacket::ip));
		   memcpy(&orangePac->node,c+sizeof(Packet::id)+sizeof(OrangePacket::ip),sizeof(Token::boolean));
		   memcpy(&orangePac->node,c+sizeof(Packet::id)+sizeof(OrangePacket::ip)+sizeof(Token::boolean),sizeof(Token::node));
		   memcpy(&orangePac->assignedIp,c+sizeof(Packet::id)+sizeof(OrangePacket::ip)+sizeof(Token::boolean)+sizeof(Token::node),sizeof(Token::assignedIp));
		   memcpy(&orangePac->assignedPort,c+sizeof(Packet::id)+sizeof(OrangePacket::ip)+sizeof(Token::boolean)+sizeof(Token::node)+sizeof(Token::assignedIp),sizeof(Token::assignedPort));
           return orangePac;
		break;
		 case static_cast<int>(ID::BCHUNK):
         blueC=(BChunk*) calloc  (1,sizeof(BChunk));
         memcpy(&blueC->id,c,sizeof(Packet::id));
         memcpy(&blueC->name[10],&c+sizeof(Packet::id),sizeof(BluePacket::name[10]));
         memcpy(&blueC->totalChunks,&c+sizeof(Packet::id)+sizeof(BluePacket::name[10]),sizeof(BChunk::totalChunks));
         memcpy(&blueC->chunkNumber,&c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BChunk::totalChunks),sizeof(BChunk::chunkNumber));
         memcpy(&blueC->data[10240],&c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BChunk::totalChunks)+sizeof(BChunk::chunkNumber),sizeof(BChunk::data[10240]));
         memcpy(&blueC->hops,&c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BChunk::totalChunks)+sizeof(BChunk::chunkNumber)+sizeof(BChunk::data[10240]),sizeof(BChunk::hops));
         return blueC;
        break;
    case static_cast<int>(ID::BHELLO):
         blueH=(BHello*) calloc  (1,sizeof(BHello));
         memcpy(&blueH->id,c,sizeof(Packet::id));
         memcpy(&blueH->name[10],c+sizeof(Packet::id),sizeof(BluePacket::name[10]));
         memcpy(&blueH->node,c+sizeof(Packet::id)+sizeof(BluePacket::name[10]),sizeof(BHello::node));
         return blueH;
    break;
    case static_cast<int>(ID::BEXIST):
         blueE=(BExist*) calloc  (1,sizeof(BExist));
         memcpy(&blueE->id,c,sizeof(Packet::id));
         memcpy(&blueE->name[10],c+sizeof(Packet::id),sizeof(BluePacket::name[10]));
         memcpy(&blueE->question,c+sizeof(Packet::id)+sizeof(BluePacket::name[10]),sizeof(BExist::question));
         memcpy(&blueE->exist,c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BExist::question),sizeof(BExist::exist));
         return blueE;
    break;
    case static_cast<int>(ID::BCOMPLETE):
         blueCom=(BComplete*) calloc  (1,sizeof(BComplete));
         memcpy(&blueCom->id,c,sizeof(Packet::id));
         memcpy(&blueCom->name[10],c+sizeof(Packet::id),sizeof(BluePacket::name[10]));
         memcpy(&blueCom->totalChunks,c+sizeof(Packet::id)+sizeof(BluePacket::name[10]),sizeof(BComplete::totalChunks));
         memcpy(&blueCom->question,c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BComplete::totalChunks),sizeof(BComplete::question));
         memcpy(&blueCom->chunkNumber,c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BComplete::totalChunks)+sizeof(BComplete::question),sizeof(BComplete::chunkNumber));
         return blueCom;
    break;
    case static_cast<int>(ID::BGET):
         blueG=(BGet*) calloc  (1,sizeof(BGet));
         memcpy(&blueG->id,c,sizeof(Packet::id));
         memcpy(&blueG->name[10],c+sizeof(Packet::id),sizeof(BluePacket::name[10]));
         memcpy(&blueG->totalChunks,c+sizeof(Packet::id)+sizeof(BluePacket::name[10]),sizeof(BGet::totalChunks));
         memcpy(&blueG->question,c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BGet::totalChunks),sizeof(BGet::question));
         memcpy(&blueG->chunkNumber,c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BGet::totalChunks)+sizeof(BGet::question)+sizeof(BGet::question),sizeof(BGet::chunkNumber));
         memcpy(&blueG->chunk[10240],c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BGet::totalChunks)+sizeof(BGet::question)+sizeof(BGet::question)+sizeof(BGet::chunkNumber),sizeof(BGet::chunk[10240]));
         return blueG;
    break;
    case static_cast<int>(ID::BLOCALIZE):
         blueL=(BLocalize*) calloc  (1,sizeof(BLocalize));
         memcpy(&blueL->id,c,sizeof(Packet::id));
         memcpy(&blueL->name[10],c+sizeof(Packet::id),sizeof(BluePacket::name[10]));
         memcpy(&blueL->question,c+sizeof(Packet::id)+sizeof(BluePacket::name[10]),sizeof(BLocalize::question));
         memcpy(&blueL->ip,c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BLocalize::question),sizeof(BLocalize::ip));
         memcpy(&blueL->port,c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BLocalize::question)+sizeof(BLocalize::ip),sizeof(BLocalize::port));
         memcpy(&blueL->chunkNumber,c+sizeof(Packet::id)+sizeof(BluePacket::name[10])+sizeof(BLocalize::question)+sizeof(BLocalize::ip)+sizeof(BLocalize::port),sizeof(BLocalize::chunkNumber));
         return blueL;
    break;
    case static_cast<int>(ID::BDELETE):
         blueD=(BDelete*) calloc  (1,sizeof(BDelete));
         memcpy(&blueD->id,c,sizeof(Packet::id));
         memcpy(&blueD->name[10],c+sizeof(Packet::id),sizeof(BluePacket::name[10]));
         return blueD;
    break;
    case static_cast<int>(ID::ASSIGNMENT):
         blueOrA=(Assignment*) calloc  (1,sizeof(Assignment));
         memcpy(&blueOrA->id,c,sizeof(Packet::id));
         memcpy(&blueOrA->nodeID,c+sizeof(Packet::id),sizeof(Assignment::nodeID));
         memcpy(&blueOrA->totalNeighbors,c+sizeof(Packet::id)+sizeof(Assignment::nodeID),sizeof(Assignment::totalNeighbors));
         memcpy(&blueOrA->ip,c+sizeof(Packet::id)+sizeof(Assignment::nodeID)+sizeof(Assignment::totalNeighbors),sizeof(Assignment::ip));
         memcpy(&blueOrA->port,c+sizeof(Packet::id)+sizeof(Assignment::nodeID)+sizeof(Assignment::totalNeighbors)+sizeof(Assignment::ip),sizeof(Assignment::port));
         return blueOrA;
    break;
    case static_cast<int>(ID::CONNECT):
         blueOrC=(Connect*) calloc  (1,sizeof(Connect));
         memcpy(&blueOrC->id,c,sizeof(Packet::id));
         return blueOrC;
    break;
    case static_cast<int>(ID::GO):
         blueOrG=(Go*) calloc  (1,sizeof(Go));
         memcpy(&blueOrG->id,c,sizeof(Packet::id));
         return blueOrG;
    break;
    case static_cast<int>(ID::GCHUNK):
         greenC=(GChunk*) calloc  (1,sizeof(GChunk));
         memcpy(&greenC->id,c,sizeof(Packet::id));
         memcpy(&greenC->name[10],c+sizeof(Packet::id),sizeof(GreenBlue::name[10]));
         memcpy(&greenC->totalChunks,c+sizeof(Packet::id)+sizeof(GreenBlue::name[10]),sizeof(GChunk::totalChunks));
         memcpy(&greenC->data[10240],c+sizeof(Packet::id)+sizeof(GreenBlue::name[10])+sizeof(GChunk::totalChunks),sizeof(GChunk::data[10240]));
         return greenC;
    break;
    case static_cast<int>(ID::GEXIST):
         greenE=(GExist*) calloc  (1,sizeof(GExist));
         memcpy(&greenE->id,c,sizeof(Packet::id));
         memcpy(&greenE->name[10],c+sizeof(Packet::id),sizeof(GreenBlue::name[10]));
         memcpy(&greenE->exist,c+sizeof(Packet::id)+sizeof(GreenBlue::name[10]),sizeof(GExist::exist));
         return greenE;
    break;
    case static_cast<int>(ID::GCOMPLETE):
         greenCom=(GComplete*) calloc  (1,sizeof(GComplete));
         memcpy(&greenCom->id,c,sizeof(Packet::id));
         memcpy(&greenCom->name[10],c+sizeof(Packet::id),sizeof(GreenBlue::name[10]));
         memcpy(&greenCom->complete,c+sizeof(Packet::id)+sizeof(GreenBlue::name[10]),sizeof(GComplete::complete));
         return greenCom;
    break;
    case static_cast<int>(ID::GDELETE):
         greenD=(GDelete*) calloc  (1,sizeof(GDelete));
         memcpy(&greenD->id,c,sizeof(Packet::id));
         memcpy(&greenD->name[10],c+sizeof(Packet::id),sizeof(GreenBlue::name[10]));
         return greenD;
    break;
    case static_cast<int>(ID::GLOCALIZE):
         greenL=(GLocalize*) calloc  (1,sizeof(GLocalize));
         memcpy(&greenL->id,c,sizeof(Packet::id));
         memcpy(&greenL->name[10],c+sizeof(Packet::id),sizeof(GreenBlue::name[10]));
         memcpy(&greenL->question,c+sizeof(Packet::id)+sizeof(GreenBlue::name[10]),sizeof(GLocalize::question));
         memcpy(&greenL->ip,c+sizeof(Packet::id)+sizeof(GreenBlue::name[10])+sizeof(GLocalize::question),sizeof(GLocalize::ip));
         memcpy(&greenL->port,c+sizeof(Packet::id)+sizeof(GreenBlue::name[10])+sizeof(GLocalize::question)+sizeof(GLocalize::ip),sizeof(GLocalize::port));
         memcpy(&greenL->chunkNumber,c+sizeof(Packet::id)+sizeof(GreenBlue::name[10])+sizeof(GLocalize::question)+sizeof(GLocalize::ip)+sizeof(GLocalize::port),sizeof(GLocalize::chunkNumber));
         return greenL;
    break;
    case static_cast<int>(ID::GKILL):
         greenK=(GKill*) calloc  (1,sizeof(GKill));
         memcpy(&greenK->id,c,sizeof(Packet::id));
         memcpy(&greenK->name[10],c+sizeof(Packet::id),sizeof(GreenBlue::name[10]));
         memcpy(&greenK->node,c+sizeof(Packet::id)+sizeof(GreenBlue::name[10]),sizeof(GKill::node));
         return greenK;
    break;

    }
    cout << "Invalid id: " << id << endl;
	return nullptr;
}
