# Descrição inicial do trabalho

Este repositório contém a implementação do método Lempel-Ziv-Welch (LZW), utilizado para realizar a compressão de arquivos sem perda.

Em um mundo amplamente conectado como o nosso, onde em todo instante estamos enviando, recebendo e armazenando muitos dados com diversos objetivos. Percebemos, então, que reduzir o tamanho desses dados é vantajoso para todos, pois isso reduz o espaço usado pelos mesmos, além de diminuir o tempo necessário para sua transferência. Para isso, podemos utilizar algoritmos de compressão de dados, com uma das diversas opção sendo o método LZW. De maneira resumida, este algorítmo busca padrões que se repetem em uma sequência, e codifica esses padrões para que, quando o arquivo for comprimido, ele ocupe menos espaço. Ele é especialmente eficiente para comprimir arquivos com muita redundância. 

O algorítmo necessita de uma estrutura de dados que permita as operações de consulta e inserção de maneira eficiente. Para isso, foi implementada uma árvore de prefíxo compacta (também chamada de Trie compacta), que mapeia sequências para sequências.

Integrantes do grupo:
- Iago Zagnoli Albergaria
- Manuel Junio Ferraz Cardoso

# Descrição dos métodos e implementações

A implementação pode ser dividida, basicamente, em três partes (essa divisão realmente ocorre no projeto, com divisões de arquivos):
- Árvore de prefixo compacta
- Algoritmo LZW
- Funções de leitura e escrita

### Árvore de prefixo compacta (Trie)


### Algoritmo LZW

O algoritmo LZW possui tanto as funções de compressão quanto de descompressão de arquivos. Em ambos os casos, fazemos o uso de um dicionário, para verificarmos a existência de uma sequência no mesmo e à qual valor ela está associada, no caso de estar no dicionário. Nesta implementação, foi utilizada a Trie citada acima. Além disso, temos funções auxiliares neste módulo, como uma função para reiniciar o dicionário, e outras para converter um caractere ou um número inteiro em uma string contendo sua representação binária.

No caso da compressão de arquivos, iniciamos o dicionário mapeando cada um dos 256 valores de byte para um código, além de uma string S representando o primeiro byte do arquivo. Após isso, algoritmo funciona da seguinte maneira: para cada outro byte no arquivo de entrada, verificamos se a string $S$ concatenada com a representação deste byte (chamaremos essa concatenação de $S_b$) pertence ao dicionário. Caso sim, substituímos $S$ por $S_b$. Caso contrário, adicionamos $S_b$ ao dicionário e substituímos $S$ pela representação do byte, além de adicionar o código associado à $S$ no dicionário à saída.

Por rasões de eficiência (especialmente em termos de memória), fazemos uso de um limite de itens que podem estar no dicionário, um número máximo $m$ de bits que os códigos podem ter. Como, para cada bit temos dois valores possíveis, percebemos que temos no máximo $2^m$ códigos possíveis. Este valor de m costuma estar entre 10 e 16 bits (valores maiores diminuem a capacidade de compressão, e valores menores aumentam o custo do algoritmo). Caso, ao tentar inserir $S$ no dicionário, percebemos que o mesmo está cheio, nós o reiniciamos para os 256 valores iniciais antes de inserir $S_b$.

É válido citar que existem duas versões do método LZW: uma fixa e outra variável. No caso da fixa, todos os códigos sempre possuem $m$ bits, enquanto na variável, os códigos se iniciam com 9 e vão crescendo conforme o necessário, limitadas à $m$ bits. Nesta implementação, essas diferenças foram tratadas da seguinte maneira: temos um valor mínimo e máximo de bits que podemos utilizar, além de uma variável armazenando a quantidade de bits utilizadas, que chamaremos de $n$. No caso da versão fixa, os três valores são sempre iguais, enquanto na variável, o mínimo possui o valor de 9 bits, o máximo é o valor $m$ informado, e $n$ é iniciado como 9, e incrementado em 1 sempre que o dicionário possui $2^n$ bits (exceto quando $n=m$, neste caso, reiniciamos $n$ como 9.

Conforme os códigos são obtidos no algoritmo, nós os armazenamos até o dicionário for reiniciado ou chegarmos ao final do arquivo. Nestes momentos, usamos as funções de escrita e leitura dos arquivos compactados, descritas posteriormente. Fazemos isso para podermos padronizar os códigos para todas terem o mesmo tamanho, tendo em vista que não conhecemos este valor quando usamos a versão variável.

Pensando agora na descompressão, fazemos um processo parecido (mas invertido, pois usaremos os códigos obtidos para recuperar o arquivo original). Para cada código no arquivo compactado, verificamos se o mesmo pertence ao dicionário (aqui, mapeando códigos para sequências): se sim, imprimimos a sequência associada à esse código; caso contrário, imprimimos a string adicionada no passo anterior concatenada com seus primeiros 8 elementos. Além disso, em ambos os casos, adicionamos a string do passo anterior com os primeiros 8 elementos da impressa neste passo (lembrando que cada elemento representa um bit, logo, os 8 bits se referem à um byte).

### Funções de leitura e escrita

Durante a execução do algoritmo, são usadas strings binárias (isto é, strings apenas de 0's ou 1's) para representar os dados do arquivo original. Entretanto, essas strings são tratadas destas maneiras apenas para facilitação durante a execução, e não podem ser escritas no arquivo comprimido desta maneira, pois cada 1 ou 0 nelas são, na verdade, caracteres (ou seja, 1 byte para representar cada bit). Por isso, foram criadas funções para facilitar a conversão dessas strings para bytes em si (no caso da escrita do arquivo compactado) e de bytes para strings (no caso de ler o arquivo compactado). Isto é, no caso da escrita, convertemos a string 10101010 (que possui 8 bytes) para o byte que possui essa representação, dividindo o espaço ocupado por 8.

Neste módulo temos, basicamente, funções com 3 objetivos:
- função que recebe bits: essa função recebe uma string binária e um inteiro. Esse inteiro é usado na padronização de tamanhos das strings, adicionando essa quantidade de 0's na "esquerda". Após isso, salvamos cada bit da string no conteúdo da classe.
  
- funções para imprimir no arquivo: temos 2 funções nesse grupo, uma para escrever o arquivo comprimido e outra para reescrever o arquivo original. A primeira, para cada vez que é chamada, escreve o tamanho de cada elemento, o número $n$ de bytes que serão escritos, e por fim os bits recebidos na função acima. os bits são convertidos em bytes, de maneira que se uma string não consuma um byte por completo, a próxima usará o final deste, além do que ela precisar. A segunda apenas escreve os bits em si (convertidos em bytes), tendo em vista que não queremos padronizar nada, apenas voltar para o formato original.
  
- função para ler: essa função é responsável por ler os dados gravados na primeira função de escrita (isto é, ler o arquivo compactado). Ela lê os bytes e os converte novamente para strings binárias, do mesmo tamanho que possuiam antes.

# Exemplos de compactação e descompactação

Para mostrar o desempenho do algoritmo, serão exibitos, nesta página, quadro arquivos diferentes como exemplos, de diferentes formatos e tamanhos: um pequeno arquivo de texto com caracteres especiais, uma imagem no formato .bmp totalmente preta (para mostrar a eficiência do algoritmo em arquivos com muita redundância), uma outra imagem no formato .bmp, mas colorida, uma base de dados no formato .csv (que na verdade é apenas um texto) e, por fim, uma imagem no formato .png, para mostrar casos em que o algoritmo não é eficiente.

### Exemplo 1: .txt com caracteres especiais

Primeiro, será testado um pequeno arquivo de texto com caracteres como emojis e acentos. O arquivo testado tem o seguinte conteúdo:

    Um arquivo de texto aleatório com alguns acentos aleatórios.
    palavras com acentos: céu chão põe paralelepípedo
    ´´´´´´´´
    ````````
    ~~~~~~~~~~~~~~~
    ^^^^^^^^^^^
    😄😶👌🙃🧑‍💻🧌🐔🎖️🐓🌎🍰

Este arquivo possui um tamanho de 225 bytes. Ao comprimir












