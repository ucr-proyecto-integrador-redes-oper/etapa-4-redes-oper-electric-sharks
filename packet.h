#ifndef PACKET_H
#define PACKET_H

class packet
{
    public:
        struct tokenBegin {//Token que crea cada naranja para ver quien gana con ip más baja
          int id;
          int ip;
          bool condition;
        };

        struct tokenMessage {//token que circula vacío, hasta que lo atrapen y cambien a ocupado
          int id;
          int ip;
          bool empty;
          char node;
          int ipBlue;
          int port;
        };

        struct ackToken {// ACK para cada token
          int ackId;//aquí es 0 o 1
          int id;
        };
        
        packet();
        virtual ~packet();
    protected:
    private:
};

#endif // PACKET_H
