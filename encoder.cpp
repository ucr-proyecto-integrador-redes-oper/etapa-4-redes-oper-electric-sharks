#include "encoder.h"
#include "error_handler.h"

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
    
    if(static_cast<OrangePacket*>(pac)){
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
         memcpy(c + sizeof(Packet::id) + sizeof(OrangePacket::ip) + sizeof(Token::boolean) + sizeof(Token::node) + sizeof(Token::assignedIp),&((Token*)pac)->assignedPort,sizeof(Token::assignedPort));
        break;
	}
	
	
    if(static_cast<BlueOrange*>(pac)){
		switch((unsigned int) id){
			 case static_cast<int>(ID::BOJOIN_GRAPH):
			  c = new char[1]();
			  memcpy(c,&pac->id,sizeof(Packet::id));
             break;
             case static_cast<int>(ID::BOGRAPH_POSITION_E):
			  c = new char[5]();
			  memcpy(c,&pac->id,sizeof(Packet::id));
			  memcpy(c + sizeof(Packet::id),&((BOGraphPosition_E*)pac)->nodeID,sizeof(BOGraphPosition_E::nodeID));
              memcpy(c + sizeof(Packet::id) + sizeof(BOGraphPosition_E::nodeID),&((BOGraphPosition_E*)pac)->neighborID,sizeof(BOGraphPosition_E::neighborID));
             break;
             case static_cast<int>(ID::BOGRAPH_POSITION_N):
			  c = new char[7]();
			  memcpy(c,&pac->id,sizeof(Packet::id));
			  memcpy(c + sizeof(Packet::id),&((BOGraphPosition_N*)pac)->neighborIP,sizeof(BOGraphPosition_N::neighborIP));
              memcpy(c + sizeof(Packet::id) + sizeof(BOGraphPosition_N::neighborIP),&((BOGraphPosition_N*)pac)->neighborPort,sizeof(BOGraphPosition_N::neighborPort));
             break;
             case static_cast<int>(ID::BOGRAPH_COMPLETE:
			  c = new char[1]();
			  memcpy(c,&pac->id,sizeof(Packet::id));
             break;
	    }
	}
	/*
    case static_cast<int>(ID::BCHUNK):
         c = new char[10262]();
         memcpy(c,&pac->id,sizeof(Packet::id));
         memcpy(c + sizeof(Packet::id),&((BluePacket*)pac)->name[10],sizeof(BluePacket::name[10]));
         memcpy(c + sizeof(Packet::id)+sizeof(BluePacket::name[10]),&((BChunk*)pac)->totalChunks,sizeof(BChunk::totalChunks));
         memcpy(c + sizeof(Packet::id)+sizeof(BluePacket::name[10]) + sizeof(BChunk::totalChunks),&((BChunk*)pac)->chunkNumber,sizeof(BChunk::chunkNumber));
         memcpy(c + sizeof(Packet::id)+sizeof(BluePacket::name[10]) + sizeof(BChunk::totalChunks) + sizeof(BChunk::chunkNumber),&((BChunk*)pac)->data[10240],sizeof(BChunk::data[10240]));
         memcpy(c + sizeof(Packet::id)+sizeof(BluePacket::name[10]) + sizeof(BChunk::totalChunks) + sizeof(BChunk::chunkNumber) + sizeof(BChunk::data[10240]),&((BChunk*)pac)->hops,sizeof(BChunk::hops));
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
	*/
	default:
		error_exit(-1, "Encode error: Id desconocido!\n");
    }
	return c;
}

Packet* Code::decode(char *c, char typePacket){
	  //Declaracion de cada struct y de ahí accede a los campos de el y de los que hereda*************
    InitialToken *orangeI;
    Token *orangePac;
    
    BOJoinGraph *BOJoin_Graph;
	BOGraphPosition_E *BOGraphPositionE ;
	BOGraphPosition_N *BOGraphPositionN;
	BOGraphComplete *BOGraph_Complete;
	/*
    BChunk *blueC;
    BHello *blueH;
    BExist *blueE;
    BComplete *blueCom;
    BGet *blueG;
    BLocalize *blueL;
    BDelete *blueD;
    * 
    GChunk *greenC;
    GExist *greenE;
    GComplete *greenCom;
    GDelete *greenD;
    GLocalize *greenL;
    GKill *greenK;
	*/
    //*****************************
	unsigned char id = 0;
	memcpy(&id, c,sizeof(Packet::id));
	if(typePacket == 0){
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
	}
	if(typePacket == 3){
	     switch((unsigned int) id){
			case static_cast<int>(ID::BOJOIN_GRAPH):
				BOJoin_Graph=(BOJoinGraph*) calloc(1,sizeof(BOJoinGraph));
				memcpy(&BOJoin_Graph->id, c,sizeof(Packet::id));
				return BOJoin_Graph;
			break;	
			case static_cast<int>(ID::BOGRAPH_POSITION_E):
				BOGraphPositionE=(BOJoinGraph*) calloc(1,sizeof(BOJoinGraph));
				memcpy(&BOGraphPositionE->id, c,sizeof(Packet::id));
				memcpy(&BOGraphPositionE->nodeID,c+sizeof(Packet::id),sizeof(BOGraphPosition_E::nodeID));
				memcpy(&BOGraphPositionE->neighborID,c+sizeof(Packet::id)+sizeof(BOGraphPosition_E::nodeID),sizeof(BOGraphPosition_E::neighborID));
				return BOGraphPositionE;
			break;
		
	}
		/*
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
	*/

    }
    error_exit(-1, "Decode error: Id desconocido!\n");
    return nullptr;
}

size_t Code::findPacketLen(const Packet* p)
{
	if(static_cast<OrangePacket*>(pac)){

		switch((unsigned int) p->id){
			case ID::INITIAL_TOKEN:
			return sizeof(InitialToken);
		break;
        case ID::TOKEN_EMPTY:
			case ID::TOKEN_FULL_AND_COMPLETE:
			case ID::TOKEN_FULL_AND_REQUEST:
			return sizeof(Token);
		break;
	}
	 if(static_cast<BlueOrange*>(pac)){
		switch((unsigned int) p->id){
			case ID::BOJOIN_GRAPH
			return sizeof(BOJoinGraph);
		break;
        case ID::BOGRAPH_POSITION_E:
			return sizeof(BOGraphPosition_E);
		break;
		case ID::BOGRAPH_POSITION_N:
			return sizeof(BOGraphPosition_N);
		break;
		case ID::BOGRAPH_COMPLETE:
			return sizeof(BOGraphComplete);
		break;
	 }
		/*
		case ID::BCHUNK:
		    return sizeof(BChunk);
		    break;
		case ID::BHELLO:
		    return sizeof(BHello);
		    break;
		case ID::BEXIST:
		    return sizeof(BExist);
		    break;
		case ID::BCOMPLETE:
		    return sizeof(BComplete);
		    break;
		case ID::BGET:
		    return sizeof(BGet);
		    break;
		case ID::BLOCALIZE:
		    return sizeof(BLocalize);
		    break;
		case ID::BDELETE:
		    return sizeof(BDelete);
		    break;
		case ID::GCHUNK:
		    return sizeof(GChunk);
		    break;
		case ID::GEXIST:
		    return sizeof(GExist);
		    break;
		case ID::GCOMPLETE:
		    return sizeof(GComplete);
		    break;
		case ID::GDELETE:
		    return sizeof(GDelete);
		    break;
		case ID::GLOCALIZE:
		    return sizeof(GLocalize);
		    break;
		case ID::GKILL:
		    return sizeof(GKill);
		    break;
		*/
		default:
			error_exit(-1, "Find packet len error: Id desconocido!\n");
	}
	return -1;
}
