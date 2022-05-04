#ifndef ALLOC_H
    #define ALLOC_H


    //Funções que retornam um endereço de memoria
    void* alocaId(uint32,uint32); // Aloca um espaço na memoria ( tamanho(bytes), id da variavel)
    void* aloca(uint32); // Aloca um espaço na memoria com id padrão 0 (tamanho(bytes))
    void* alocaSuper(uint32); // Aloca um espaço na memoria com id padrão ID_SUPER (tamanho(bytes))
    void* alocaTemp(uint32); // Envia como retorno um endereço na memoria vazio com o tamanho solicitado, porém ele não reserva na memoria e qualquer outra variavel pode sobrescreve-lo (tamanho(bytes))
    void* maloca(uint32); // Aloca um espaço na memoria zerado com id padrão 0 (tamanho(bytes))
    void* realoca(void*,uint32); // Redefine um novo espaço na memoria mantendo o id (endereço variavel,tam)
    void* getEndVoid(); // Retorna um endereço com tamanho de 64 bytes **ESSE ENDEREÇO NUNCA DEVE SER USADO**
    void* getEndPublic(); // Retorna o endereço do vetor da memoria
    void* alocaCop(void *,uint32); // Passa um endereço como parametro que vai ser copiado e gerado um novo endereço com os msm dados porém em outro local, o id é dinamico;

    //Utils
    uint32 getIdVer(); // Retorna uma id válida e nunca utilizada
    boolean verId(uint32); // Verifica se uma id é válida e nunca foi utilizada
    uint32 getId(); // Recebe uma id válida
    boolean isSpace(uint32); // Verifica se há espaço na memória com o tamanho passado no parametro (tamanho(bytes))
    int getBit(uint8,int); // Retorna o valor de um bit na posição enviada do byte enviado ( byte , posição )


    //Procedimentos
    void resetMem();  // Reseta a memoria
    void libera(void*); // Libera um espaço de memória reservado passado como parametro (endereço variavel)
    void liberaId(uint32); // Libera toda a memoria com o id = Id_parametro (Id_parametro)
    void statusMem(); // Printa na tela o status da memória
    void liberaSuper(); // Libera toda a memória com o id = ID_SUPER
    void mostrar(); // Mostra tudo que tem na memória e o mapa de bits 1=ocupado 0=livre



#endif
