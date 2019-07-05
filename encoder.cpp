#include "encoder.h"
#include "error_handler.h"

#include <cassert>

#include <iostream>
using namespace std;

Code::Code()
{
}

Code::~Code()
{
    //dtor
}
char* Code::encode(Packet *pac, int type){
    char *c;
    unsigned char id = pac->id;
    
    switch(type){
		case NODE_ORANGE:
			switch((unsigned int) id){
			case static_cast<int>(ID::INITIAL_TOKEN):
				 c = new char[6]();
				 memcpy(c, &pac->id,sizeof(Packet::id));
				 memcpy(c + sizeof(Packet::id),&((OrangePacket*)pac)->ip,sizeof(OrangePacket::ip));
			break;
			case static_cast<int>(ID::TOKEN_EMPTY):
			case static_cast<int>(ID::TOKEN_FULL_AND_REQUEST):
			case static_cast<int>(ID::TOKEN_FULL_AND_COMPLETE):
				 c = new char[15]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c + sizeof(Packet::id),&((OrangePacket*)pac)->ip,sizeof(OrangePacket::ip));
				 memcpy(c + sizeof(Packet::id) + sizeof(OrangePacket::ip),&((Token*)pac)->boolean,sizeof(Token::boolean));
				 memcpy(c + sizeof(Packet::id) + sizeof(OrangePacket::ip) + sizeof(Token::boolean),&((Token*)pac)->node,sizeof(Token::node));
				 memcpy(c + sizeof(Packet::id) + sizeof(OrangePacket::ip) + sizeof(Token::boolean) + sizeof(Token::node),&((Token*)pac)->assignedIp,sizeof(Token::assignedIp));
				 memcpy(c + sizeof(Packet::id) + sizeof(OrangePacket::ip) + sizeof(Token::boolean) + sizeof(Token::node) + sizeof(Token::assignedIp),&((Token*)pac)->assignedPort,sizeof(Token::assignedPort));
			break;
		}
		break;
		
		case NODE_BLUE:
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
				 case static_cast<int>(ID::BOGRAPH_COMPLETE):
				  c = new char[1]();
				  memcpy(c,&pac->id,sizeof(Packet::id));
				 break;
				 case static_cast<int>(ID::BCHUNK):
				 c = new char[1032]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c + sizeof(Packet::id),&((BChunk*)pac)->archID[3],sizeof(BChunk::archID[3]));
				 memcpy(c + sizeof(Packet::id)+sizeof(BChunk::archID[3]),&((BChunk*)pac)->chunkNum,sizeof(BChunk::chunkNum));
				 memcpy(c + sizeof(Packet::id)+sizeof(BChunk::archID[3]) + sizeof(BChunk::chunkNum),&((BChunk*)pac)->chunk[MAX_PAYLOAD_SIZE],sizeof(BChunk::chunk[MAX_PAYLOAD_SIZE]));
			 break;	
			 case static_cast<int>(ID::BHELLO):
				 c = new char[3]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BHello*)pac)->nodeID,sizeof(BHello::nodeID));
			 break;
			 case static_cast<int>(ID::BEXISTS_R):
				 c = new char[8]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BExist_R*)pac)->archID[3],sizeof(BExist_R::archID[3]));
				 memcpy(c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),&((BExist_R*)pac)->chunkNum,sizeof(BExist_R::chunkNum));
			 break;
			 case static_cast<int>(ID::BEXISTS_A):
				 c = new char[8]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BExist_R*)pac)->archID[3],sizeof(BExist_R::archID[3]));
				 memcpy(c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),&((BExist_R*)pac)->chunkNum,sizeof(BExist_R::chunkNum));
			 break;
			 case static_cast<int>(ID::BCOMPLETE_R):
				 c = new char[8]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BExist_R*)pac)->archID[3],sizeof(BExist_R::archID[3]));
				 memcpy(c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),&((BExist_R*)pac)->chunkNum,sizeof(BExist_R::chunkNum));
             break;
             case static_cast<int>(ID::BCOMPLETE_A):
				 c = new char[8]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BExist_R*)pac)->archID[3],sizeof(BExist_R::archID[3]));
				 memcpy(c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),&((BExist_R*)pac)->chunkNum,sizeof(BExist_R::chunkNum));
             break;
             case static_cast<int>(ID::BGET_A):
				 c = new char[8]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BExist_R*)pac)->archID[3],sizeof(BExist_R::archID[3]));
				 memcpy(c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),&((BExist_R*)pac)->chunkNum,sizeof(BExist_R::chunkNum));
             break;
             case static_cast<int>(ID::BGET_R):
				 c = new char[8]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BExist_R*)pac)->archID[3],sizeof(BExist_R::archID[3]));
				 memcpy(c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),&((BExist_R*)pac)->chunkNum,sizeof(BExist_R::chunkNum));
             break;
             case static_cast<int>(ID::BLOCATE_A):
				 c = new char[6]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BLocate_A*)pac)->nodeID,sizeof(BLocate_A::nodeID));
				 memcpy(c+sizeof(Packet::id)+sizeof(BLocate_A::nodeID),&((BLocate_A*)pac)->archID[3],sizeof(BLocate_A::archID[3]));
             break;
             case static_cast<int>(ID::BLOCATE_R):
				 c = new char[8]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BExist_R*)pac)->archID[3],sizeof(BExist_R::archID[3]));
				 memcpy(c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),&((BExist_R*)pac)->chunkNum,sizeof(BExist_R::chunkNum));
             break;
             case static_cast<int>(ID::BDELETE):
				 c = new char[4]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BDelete*)pac)->archID[3],sizeof(BDelete::archID[3]));
             break;
             case static_cast<int>(ID::BJOIN_TREE):
				 c = new char[3]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BHello*)pac)->nodeID,sizeof(BHello::nodeID));
             break;
             case static_cast<int>(ID::BIDO):
				 c = new char[3]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BHello*)pac)->nodeID,sizeof(BHello::nodeID));
             break;
             case static_cast<int>(ID::BIDONOT):
				 c = new char[3]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BHello*)pac)->nodeID,sizeof(BHello::nodeID));
             break;
             case static_cast<int>(ID::BDADDY):
                 c = new char[3]();
				 memcpy(c,&pac->id,sizeof(Packet::id));
				 memcpy(c+sizeof(Packet::id),&((BHello*)pac)->nodeID,sizeof(BHello::nodeID));
             break;
				 default:
				 assert(false);
				 error_exit(-1, "Encode error: Id desconocido!\n");
			 }
			break;
	    default:
			assert(false);
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
	BChunk *Bchunk;
	BDelete *Bdelete;
	BHello *Bhello;
	BJoin *Bjoin;
	BIdo *Bido;
	BIdoNot *BidoNot;
	BDaddy *Bdaddy;
	BExist_R *Bexist_R;
	BExist_A *Bexist_A;
	BComplete_R *Bcomplete_R;
	BGet_R *Bget_R;
	BGet_A *Bget_A;
	BLocate_R *Blocate_R;
	BComplete_A *Bcomplete_A;
	BLocate_A *Blocate_A;
	
	unsigned char id = 0;
	memcpy(&id, c,sizeof(Packet::id));
	
	if(typePacket == PACKET_ORANGE){
		switch((unsigned int) id){
			case static_cast<int>(ID::INITIAL_TOKEN):
				orangeI=(InitialToken*) calloc(1,sizeof(InitialToken));
				memcpy(&orangeI->id, c,sizeof(Packet::id));
				memcpy(&orangeI->ip,c+sizeof(Packet::id),sizeof(OrangePacket::ip));
				return orangeI;
			break;
			case static_cast<int>(ID::TOKEN_EMPTY):
			case static_cast<int>(ID::TOKEN_FULL_AND_REQUEST):
			case static_cast<int>(ID::TOKEN_FULL_AND_COMPLETE):
				orangePac=(Token*) calloc  (1,sizeof(Token));
				memcpy(&orangePac->id,c,sizeof(Packet::id));
				memcpy(&orangePac->ip,c+sizeof(Packet::id),sizeof(OrangePacket::ip));
				memcpy(&orangePac->boolean,c+sizeof(Packet::id)+sizeof(OrangePacket::ip),sizeof(Token::boolean));
				memcpy(&orangePac->node,c+sizeof(Packet::id)+sizeof(OrangePacket::ip)+sizeof(Token::boolean),sizeof(Token::node));
				memcpy(&orangePac->assignedIp,c+sizeof(Packet::id)+sizeof(OrangePacket::ip)+sizeof(Token::boolean)+sizeof(Token::node),sizeof(Token::assignedIp));
				memcpy(&orangePac->assignedPort,c+sizeof(Packet::id)+sizeof(OrangePacket::ip)+sizeof(Token::boolean)+sizeof(Token::node)+sizeof(Token::assignedIp),sizeof(Token::assignedPort));
				return orangePac;
			break;
		}
	}
	else if(typePacket == PACKET_BLUE){
	     switch((unsigned int) id){
			case static_cast<int>(ID::BOJOIN_GRAPH):
				BOJoin_Graph = (BOJoinGraph*) calloc(1,sizeof(BOJoinGraph));
				memcpy(&BOJoin_Graph->id, c,sizeof(Packet::id));
				return BOJoin_Graph;
			break;	
			case static_cast<int>(ID::BOGRAPH_POSITION_E):
				BOGraphPositionE = (BOGraphPosition_E*) calloc(1,sizeof(BOGraphPosition_E));
				memcpy(&BOGraphPositionE->id, c,sizeof(Packet::id));
				memcpy(&BOGraphPositionE->nodeID,c+sizeof(Packet::id),sizeof(BOGraphPosition_E::nodeID));
				memcpy(&BOGraphPositionE->neighborID,c+sizeof(Packet::id)+sizeof(BOGraphPosition_E::nodeID),sizeof(BOGraphPosition_E::neighborID));
				return BOGraphPositionE;
			break;
			case static_cast<int>(ID::BOGRAPH_POSITION_N):
				BOGraphPositionN = (BOGraphPosition_N*) calloc(1,sizeof(BOGraphPosition_N));
				memcpy(&BOGraphPositionN->id, c,sizeof(Packet::id));
				memcpy(&BOGraphPositionN->nodeID,c+sizeof(Packet::id),sizeof(BOGraphPosition_E::nodeID));
				memcpy(&BOGraphPositionN->neighborID,c+sizeof(Packet::id)+sizeof(BOGraphPosition_E::nodeID),sizeof(BOGraphPosition_E::neighborID));
				memcpy(&BOGraphPositionN->neighborIP,c+sizeof(Packet::id)+sizeof(BOGraphPosition_E::nodeID) + sizeof(BOGraphPosition_E::neighborID),sizeof(BOGraphPosition_N::neighborIP));
				memcpy(&BOGraphPositionN->neighborPort, c + sizeof(Packet::id) + sizeof(BOGraphPosition_E::nodeID) + sizeof(BOGraphPosition_N::neighborID)+ sizeof(BOGraphPosition_N::neighborIP) + sizeof(BOGraphPosition_N::neighborPort),sizeof(BOGraphPosition_N::neighborPort));
				return BOGraphPositionN;
			break;
			case static_cast<int>(ID::BOGRAPH_COMPLETE):
				BOGraph_Complete = (BOGraphComplete*) calloc(1,sizeof(BOGraphComplete));
				memcpy(&BOGraph_Complete->id, c,sizeof(Packet::id));
				return BOGraph_Complete;
			break;	
			case static_cast<int>(ID::BCHUNK):
				 Bchunk = (BChunk*) calloc  (1,sizeof(BChunk));
				 memcpy(&Bchunk->id,c,sizeof(Packet::id));
				 memcpy(&Bchunk->archID[3],&c+sizeof(Packet::id),sizeof(BChunk::archID[3]));
				 memcpy(&Bchunk->chunkNum,&c+sizeof(Packet::id)+sizeof(BChunk::archID[3]),sizeof(BChunk::chunkNum));
				 memcpy(&Bchunk->chunk[MAX_PAYLOAD_SIZE],&c+sizeof(Packet::id)+sizeof(BChunk::archID[3])+sizeof(BChunk::chunkNum),sizeof(BChunk::chunk[MAX_PAYLOAD_SIZE]));
				 return Bchunk;
			break;
			 case static_cast<int>(ID::BHELLO):
				 Bhello = (BHello*) calloc  (1,sizeof(BHello));
				 memcpy(&Bhello ->id,c,sizeof(Packet::id));
				 memcpy(&Bhello ->nodeID,c+sizeof(Packet::id),sizeof(BHello::nodeID));
				 return Bhello;
			break;
			case static_cast<int>(ID::BEXISTS_A):
				 Bexist_A=(BExist_A*) calloc  (1,sizeof(BExist_A));
				 memcpy(&Bexist_A->id,c,sizeof(Packet::id));
				 memcpy(&Bexist_A->archID[3],c+sizeof(Packet::id),sizeof(BExist_R::archID[3]));
				 memcpy(&Bexist_A->chunkNum,c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),sizeof(BExist_R::chunkNum));
				 return Bexist_A;
			break;
			case static_cast<int>(ID::BEXISTS_R):
				 Bexist_R=(BExist_R*) calloc  (1,sizeof(BExist_R));
				 memcpy(&Bexist_R->id,c,sizeof(Packet::id));
				 memcpy(&Bexist_R->archID[3],c+sizeof(Packet::id),sizeof(BExist_R::archID[3]));
				 memcpy(&Bexist_R->chunkNum,c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),sizeof(BExist_R::chunkNum));
				 return Bexist_R;
			break;
			case static_cast<int>(ID::BCOMPLETE_A):
				 Bcomplete_A = (BComplete_A*) calloc  (1,sizeof(BComplete_A));
				 memcpy(&Bcomplete_A->id,c,sizeof(Packet::id));
				 memcpy(&Bcomplete_A->archID[3],c+sizeof(Packet::id),sizeof(BExist_R::archID[3]));
				 memcpy(&Bcomplete_A->chunkNum,c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),sizeof(BExist_R::chunkNum));
				 return Bcomplete_A;
			break;
			case static_cast<int>(ID::BCOMPLETE_R):
				 Bcomplete_R = (BComplete_R*) calloc  (1,sizeof(BComplete_R));
				 memcpy(&Bcomplete_R->id,c,sizeof(Packet::id));
				 memcpy(&Bcomplete_R->archID[3],c+sizeof(Packet::id),sizeof(BExist_R::archID[3]));
				 memcpy(&Bcomplete_R->chunkNum,c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),sizeof(BExist_R::chunkNum));
				 return Bcomplete_R;
			break;
			case static_cast<int>(ID::BGET_A):
				 Bget_A = (BGet_A*) calloc  (1,sizeof(BGet_A));
				 memcpy(&Bget_A->id,c,sizeof(Packet::id));
				 memcpy(&Bget_A->archID[3],c+sizeof(Packet::id),sizeof(BExist_R::archID[3]));
				 memcpy(&Bget_A->chunkNum,c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),sizeof(BExist_R::chunkNum));
				 return Bget_A;
			break;
			case static_cast<int>(ID::BGET_R):
				 Bget_R = (BGet_R*) calloc  (1,sizeof(BGet_R));
				 memcpy(&Bget_R->id,c,sizeof(Packet::id));
				 memcpy(&Bget_R->archID[3],c+sizeof(Packet::id),sizeof(BExist_R::archID[3]));
				 memcpy(&Bget_R->chunkNum,c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),sizeof(BExist_R::chunkNum));
				 return Bget_R;
			break;
			case static_cast<int>(ID::BLOCATE_A):
				 Blocate_A = (BLocate_A*) calloc  (1,sizeof(BLocate_A));
				 memcpy(&Blocate_A->id,c,sizeof(Packet::id));
				 memcpy(&Blocate_A->nodeID,c+sizeof(Packet::id),sizeof(BLocate_A::nodeID));
				 memcpy(&Blocate_A->archID[3],c+sizeof(Packet::id)+sizeof(BLocate_A::nodeID),sizeof(BLocate_A::archID[3]));
				 return Blocate_A;
			break;
			case static_cast<int>(ID::BLOCATE_R):
				 Blocate_R = (BLocate_R*) calloc  (1,sizeof(BLocate_R));
				 memcpy(&Blocate_R->id,c,sizeof(Packet::id));
				 memcpy(&Blocate_R->archID[3],c+sizeof(Packet::id),sizeof(BExist_R::archID[3]));
				 memcpy(&Blocate_R->chunkNum,c+sizeof(Packet::id)+sizeof(BExist_R::archID[3]),sizeof(BExist_R::chunkNum));
				 return Blocate_R;
			break;
			case static_cast<int>(ID::BDELETE):
				 Bdelete = (BDelete*) calloc  (1,sizeof(BDelete));
				 memcpy(&Bdelete->id,c,sizeof(Packet::id));
				 memcpy(&Bdelete->archID[3],c+sizeof(Packet::id),sizeof(BDelete::archID[3]));
				 return Bdelete;
			break;
			case static_cast<int>(ID::BJOIN_TREE):
				 Bjoin = (BJoin*) calloc  (1,sizeof(BJoin));
				 memcpy(&Bjoin->id,c,sizeof(Packet::id));
				 memcpy(&Bjoin->nodeID,c+sizeof(Packet::id),sizeof(BHello::nodeID));
				 return Bjoin;
			break;
			case static_cast<int>(ID::BIDO):
				 Bido = (BIdo*) calloc  (1,sizeof(BIdo));
				 memcpy(&Bido->id,c,sizeof(Packet::id));
				 memcpy(&Bido->nodeID,c+sizeof(Packet::id),sizeof(BHello::nodeID));
				 return Bido;
			break;
			case static_cast<int>(ID::BIDONOT):
				 BidoNot = (BIdoNot*) calloc  (1,sizeof(BIdoNot));
				 memcpy(&BidoNot->id,c,sizeof(Packet::id));
				 memcpy(&BidoNot->nodeID,c+sizeof(Packet::id),sizeof(BHello::nodeID));
				 return BidoNot;
			break;
			case static_cast<int>(ID::BDADDY):
				 Bdaddy = (BDaddy*) calloc  (1,sizeof(BDaddy));
				 memcpy(&Bdaddy->id,c,sizeof(Packet::id));
				 memcpy(&Bdaddy->nodeID,c+sizeof(Packet::id),sizeof(BHello::nodeID));
				 return Bdaddy;
			break;
		}
	  }
	assert(false);
    error_exit(-1, "Decode error: Id desconocido!\n");
    return nullptr;
}

size_t Code::findPacketLen(Packet *p)
{
	if(static_cast<OrangePacket*>(p)){

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
  }
	 if(static_cast<BlueOrange*>(p)){
		switch((unsigned int) p->id){
			case ID::BOJOIN_GRAPH:
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
		case ID::BCHUNK:
		    return sizeof(BChunk);
		    break;
		case ID::BHELLO:
		    return sizeof(BHello);
		    break;
		case ID::BEXISTS_A:
		    return sizeof(BExist_A);
		    break;
		case ID::BEXISTS_R:
		    return sizeof(BExist_R);
		    break;
		case ID::BCOMPLETE_A:
		    return sizeof(BComplete_A);
		    break;
		case ID::BCOMPLETE_R:
		    return sizeof(BComplete_R);
		    break;
		case ID::BGET_A:
		    return sizeof(BGet_A);
		    break;
		case ID::BGET_R:
		    return sizeof(BGet_R);
		    break;
		case ID::BLOCATE_A:
		    return sizeof(BLocate_A);
		    break;
		case ID::BLOCATE_R:
		    return sizeof(BLocate_R);
		    break;
		case ID::BDELETE:
		    return sizeof(BDelete);
		    break;
		case ID::BJOIN_TREE:
		    return sizeof(BJoin);
		    break;
		case ID::BIDO:
		    return sizeof(BIdo);
		    break;
		case ID::BIDONOT:
		    return sizeof(BIdoNot);
		    break;
		case ID::BDADDY:
		    return sizeof(BDaddy);
		    break;
		default:
			error_exit(-1, "Find packet len error: Id desconocido!\n");
	 }
   }
	return -1;
}
