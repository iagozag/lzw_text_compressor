# Descrição inicial do trabalho

Este repositório contém a implementação do método Lempel-Ziv-Welch (LZW), utilizado para realizar a compressão de arquivos sem perda.

Em um mundo amplamente conectado como o nosso, em todo instante estamos enviando, recebendo e armazenando muitos dados com diversos objetivos. Percebemos, então, que reduzir o tamanho desses dados é vantajoso para todos, pois isso reduz o espaço usado pelos mesmos, além de diminuir o tempo necessário para sua transferência. Para isso, podemos utilizar algoritmos de compressão de dados, com uma das diversas opção sendo o método LZW. De maneira resumida, este algoritmo busca padrões que se repetem em uma sequência, e codifica esses padrões para que, quando o arquivo for comprimido, ele ocupe menos espaço. Ele é especialmente eficiente para comprimir arquivos com muita redundância. 

O algoritmo necessita de uma estrutura de dados que permita as operações de consulta e inserção de maneira eficiente. Para isso, foi implementada uma árvore de prefixos compacta (também chamada de Trie compacta), que mapeia sequências para sequências.

Integrantes do grupo:
- Iago Zagnoli Albergaria
- Manuel Junio Ferraz Cardoso

Site para execução online do programa: https://lzw-compress.streamlit.app/

# Descrição dos métodos e implementações

A implementação pode ser dividida, basicamente, em três partes (essa divisão realmente ocorre no projeto, com divisões de arquivos no código fonte):
- Árvore de prefixo compacta
- Algoritmo LZW
- Funções de leitura e escrita

### Árvore de prefixo compacta (Trie)

Para o funcionamento do algoritmo, é necessária o uso de uma estrutura de dados para representar um dicionário. Essa estrutura deve permitir operações de inserção e remoção de strings binárias, a verificação de se ela contém uma string de maneira eficiente, além de, caso positivo, verificar a qual valor este par é associado. Como o método LZW precisa de dois dicionários diferentes (um de sequências para inteiros na compressão, e outro de inteiros para sequências na descompressão), ambos os valores foram transformados em strings binárias, para que a mesma implementação do dicionário pudesse ser utilizada nos dois casos.

Para isto, foi implementada uma árvore de prefixo compacta, também chamada de Trie compacta, pois esta estrutura permite as operações de maneira eficiente, além de economizar a memória utilizada. A versão implementada é a que utiliza um vetor que contém todos os nós, e cada nó contém o índice dos seus filhos para a esqueda e direita (apenas dois filhos, pois as strings são binárias). Esta implementação é mais eficiente do que criar, de fato, uma árvore, pois fazemos acessos mais eficientes na memória.

A Trie implementada possui cinco funções principais:
- uma função que apenas retorna o número de elementos inseridos
- uma função que insere um par (chave, valor) na Trie, ou altera o valor associado à chave, caso esta já esteja presente na estrutura
- uma função que deleta uma chave da Trie, caso tenha sido inserido
- uma função que retorna o valor associado à uma chave dada, ou uma string vazia caso esta chave não pertença à estrutura
- por fim, uma função que exclui todos os elementos armazenados (usada, principalmente, nas reinicializações do dicionário, que serão descritas posteriormente)

### Algoritmo LZW

O algoritmo LZW possui tanto as funções de compressão quanto de descompressão de arquivos. Em ambos os casos, fazemos o uso de um dicionário, para verificarmos a existência de uma sequência no mesmo e à qual valor ela está associada, no caso de estar no dicionário. Nesta implementação, foi utilizada a Trie citada acima. Além disso, temos funções auxiliares neste módulo, como funções para reiniciar o dicionário, e outras para converter um caractere ou um número inteiro em uma string contendo sua representação binária.

No caso da compressão de arquivos, iniciamos o dicionário mapeando cada um dos 256 valores de byte para um código, além de uma string S representando o primeiro byte do arquivo. Após isso, algoritmo funciona da seguinte maneira: para cada outro byte no arquivo de entrada, verificamos se a string $S$ concatenada com a representação deste byte (chamaremos essa concatenação de $S_b$) pertence ao dicionário. Caso sim, substituímos $S$ por $S_b$. Caso contrário, adicionamos $S_b$ ao dicionário e substituímos $S$ pela representação do byte, além de adicionar o código associado à $S$ no dicionário à saída.

Por razões de eficiência (especialmente em termos de memória), fazemos uso de um limite de itens que podem estar no dicionário, um número máximo $m$ de bits que os códigos podem ter. Como, para cada bit temos dois valores possíveis, percebemos que temos no máximo $2^m$ códigos possíveis. Este valor de m costuma estar entre 10 e 16 bits (valores maiores diminuem a capacidade de compressão, e valores menores aumentam o custo do algoritmo). Caso, ao tentar inserir $S$ no dicionário, percebemos que o mesmo está cheio, nós o reiniciamos para os 256 valores iniciais antes de inserir $S_b$.

É válido citar que existem duas versões do método LZW: uma fixa e outra variável. No caso da fixa, todos os códigos sempre possuem $m$ bits, enquanto na variável, os códigos se iniciam com 9 e vão crescendo conforme o necessário, limitadas à $m$ bits. Nesta implementação, essas diferenças foram tratadas da seguinte maneira: temos um valor mínimo e máximo de bits que podemos utilizar, além de uma variável armazenando a quantidade de bits utilizados, que chamaremos de $n$. No caso da versão fixa, os três valores são sempre iguais, enquanto na variável, o mínimo possui o valor de 9 bits, o máximo é o valor $m$ informado, e $n$ é iniciado como 9, e incrementado em 1 sempre que o dicionário possui $2^n$ bits (exceto quando $n=m$, neste caso, reiniciamos $n$ como 9.

Conforme os códigos são obtidos no algoritmo, nós os armazenamos até o dicionário for reiniciado ou chegarmos ao final do arquivo. Nestes momentos, usamos as funções de escrita e leitura dos arquivos compactados, descritas posteriormente. Fazemos isso para podermos padronizar os códigos para todas terem o mesmo tamanho, tendo em vista que não conhecemos este valor quando usamos a versão variável.

Pensando agora na descompressão, fazemos um processo parecido (mas invertido, pois usaremos os códigos obtidos para recuperar o arquivo original). Para cada código no arquivo compactado, verificamos se o mesmo pertence ao dicionário (aqui, mapeando códigos para sequências): se sim, imprimimos a sequência associada à esse código; caso contrário, imprimimos a string adicionada no passo anterior concatenada com seus primeiros 8 elementos. Além disso, em ambos os casos, adicionamos a string do passo anterior com os primeiros 8 elementos da impressa neste passo (lembrando que cada elemento representa um bit, logo, os 8 bits se referem à um byte).

### Funções de leitura e escrita

Durante a execução do algoritmo, são usadas strings binárias (isto é, strings apenas de 0's ou 1's) para representar os dados do arquivo original. Entretanto, essas strings são tratadas destas maneiras apenas para a facilitação durante a execução, e não podem ser escritas no arquivo comprimido desta maneira, pois cada 1 ou 0 nelas são, na verdade, caracteres (ou seja, 1 byte para representar cada bit). Por isso, foram criadas funções para facilitar a conversão dessas strings para bytes em si (no caso da escrita do arquivo compactado) e de bytes para strings (no caso de ler o arquivo compactado). Por exemplo, no caso da escrita, convertemos a string 10101010 (que possui 8 bytes) para o byte que possui essa representação, dividindo o espaço ocupado por 8, e no caso da leitura, convertemos a representação binária 10101010 (1 byte) para a string de 8 bytes contendo estes bits como caracteres.

Neste módulo temos, basicamente, funções com 3 objetivos:
- função que recebe bits: essa função recebe uma string binária e um inteiro. Esse inteiro é usado na padronização de tamanhos das strings, adicionando essa quantidade de 0's na "esquerda". Após isso, salvamos cada elemento da string no conteúdo da classe.
  
- funções para imprimir no arquivo: temos 2 funções nesse grupo, uma para escrever o arquivo comprimido e outra para reescrever o arquivo original. A primeira, para cada vez que é chamada, escreve o tamanho de cada elemento, o número $n$ de bytes que serão escritos, e por fim os bits recebidos na função acima. Os bits são convertidos em bytes, de maneira que se uma string não consuma um byte por completo, a próxima usará o final deste, além do que ela precisar. A segunda apenas escreve os bits em si (convertidos em bytes), tendo em vista que não queremos padronizar nada para leituras futuras, apenas voltar para o formato original.
  
- função para ler: essa função é responsável por ler os dados gravados na primeira função de escrita (isto é, ler o arquivo compactado). Ela lê os bytes e os converte novamente para strings binárias, do mesmo tamanho que possuiam antes.

# Exemplos de compactação e descompactação

Para mostrar o desempenho do algoritmo, serão exibitos, nesta página, cinco arquivos diferentes como exemplos, de diferentes formatos e tamanhos: um pequeno arquivo de texto com caracteres especiais, uma imagem no formato .bmp totalmente preta (para mostrar a eficiência do algoritmo em arquivos com muita redundância), uma outra imagem no formato .bmp, mas colorida, uma base de dados no formato .csv e, por fim, uma imagem no formato .png, para mostrar um caso em que o algoritmo não é eficiente. Todos os exemplos utilizarão a versão de tamanho variável do algoritmo, com o máximo de bits igual a 12.

### Exemplo 1: .txt com caracteres especiais

Primeiro, será testado um pequeno arquivo de texto com caracteres como emojis e acentos. O arquivo testado tem o seguinte conteúdo:

    Um arquivo de texto aleatório com alguns acentos aleatórios.
    palavras com acentos: céu chão põe paralelepípedo
    ´´´´´´´´
    ````````
    ~~~~~~~~~~~~~~~
    ^^^^^^^^^^^
    😄😶👌🙃🧑‍💻🧌🐔🎖️🐓🌎🍰

Este arquivo possui um tamanho de 225 bytes. Ao comprimir, obtemos um arquivo com 185 bytes, uma compressão de aproximadamente 17% (um valor baixo, mas explicável, pois a compressão se torna mais eficiente conforme o algoritmo for avançando). O objetivo desta compressão é mostrar, apenas, que caracteres especiais não são um problema para o algoritmo.

Como o arquivo é muito pequeno, o algoritmo termina extremamente rápido: apenas 1 ms foi utilizado.

### Exemplo 2: imagem totalmente preta no formato .bmp

Agora, será mostrada uma especialidade do algoritmo: comprimir arquivos com grande repetições de padrôes. A imagem comprimida será a seguinte (na verdade, a abaixo é uma versão em .png, pois o formato .bmp não foi surpotado aqui) : 

<p align="center">
  <img src="https://github.com/user-attachments/assets/3187d7df-d55f-4ebf-a328-5c1fe4e28bb6" width="500">
</p>

Neste exemplo, a imagem original ocupa 257 KB de armazenamento, porém, o arquivo comprimido possui incríveis 977 bytes. Isso gera, uma taxa de compressão de mais de 99%! Claro, este não é um exemplo muito real, e foi citado apenas para mostrar a especialidade do algoritmo.

Neste caso, a compressão foi concluída em 8,6 segundos.

### Exemplo 3: imagem colorida no formato .bmp

Agora, um exemplo mais real: uma outra imagem no formato .bmp, mas colorida (a versão abaixo também está em .png, mas a análise será sobre a .bmp):

<p align="center">
  <img src="https://github.com/user-attachments/assets/bb85f945-496f-4d3b-90ea-83e4e8445c9a" width="500">
</p>

Essa imagem possui um tamanho de 2.74 MB, enquanto sua versão comprimida possui 1.88 MB. Isso representa uma taxa de compressão de aproximadamente 31%, um valor mais baixo por conta da grande variação de cores na mesma. A compressão desta imagem ocorreu em 11 segundos.

### Exemplo 4: base de dados
Mais um exemplo real, mas maior: uma base de dados obtida em uma página de dados do governo, no formato .csv (que é, na verdade, um arquivo de texto comum com separações, geralmente por vírgula). Por conta do tamanho do arquivo, ele não será exibido aqui.

Este arquivo possui um tamanho de 46.3 MB no formato original, mas conseguimos o comprimir para ocupar apenas 19,9 MB, o que representa uma taxa de compressão de 57%. Conforme dito anteriormente, arquivos maiores e com mais repetições tendem a ser mais comprimidos, como é o caso deste exemplo em relação ao último. Por conta do arquivo original ser muito maior que os demais testes, o tempo de execução do mesmo foi de 2 minutos e 39 segundos.

## Exemplo 5: imagem no formato .png

Para finalizar, agora um exemplo em que o arquivo não é eficiente: uma imagem no formato .png.

<p align="center">
  <img src="https://github.com/user-attachments/assets/d78a2bf0-70c3-4c62-9938-ba89a1f6b3a6" width="500">
</p>

Esta imagem possui, originalmente, 2.56 MB. Porém, o arquivo "comprimido" possui o tamanho de 3,68 MB, que indica uma taxa de compressão de -43%. Nos casos em que o algoritmo não encontre grandes padrões no arquivo, isto pode ocorrer: o arquivo original ser menor que o compactado. 

Esta última imagem de exemplo teve a sua compressão concluída em, aproximadamente, 16 segundos.

# Instruções para compilação e execução

Caso queira compilar e executar o programa localmente, basta clonar o repositório. Após isso, na pasta /lzw_text_compressor, no terminal, digite os comandos abaixo:

    make
    ./main <file> (--compress / --decompress) [--max-bits <value> (12 by default)] [--stats (false by default)] [--fixed (false by default)]

O comando make é utilizado para a compilação, portanto, só é preciso ser usado uma vez. Já o segundo serve para executar de fato o programa. Neste último, devem ser dados parâmetros adicionais:
- <file\> : arquivo que será comprimido ou descomprimido. No primeiro caso, ele deve estar na pasta inputs, e o resultado estará na pasta outputs. No segundo caso, o arquivo deve estar na pasta outputs (note a terminação .lzw para arquivos comprimidos), e o resultado será armazenado na pasta inputs.
- (--compress / --decompress) : indicação se o objetivo é comprimir ou descomprimir um arquivo
- [--max-bits <value> (12 by default)\] : tamanho máximo de bits usado pelos códigos no dicionário (opcional, 12 caso não informado)
- [--stats (false by default)\] : opção para visualização de estatísticas sobre a execução do programa (opcional, falso caso não informado)
- [--fixed (false by default)\] : opção para indicar se o algoritmo será utilizado na versão fixa ou variável (opcional, variável caso não informado)

Há também uma versão para execução online do programa, disponível no link https://lzw-compress.streamlit.app/

Na página, será possível enviar um arquivo para ser comprimido ou descomprimido conforme a opção selecionada, e fazer o download do arquivo resultante. Há também botões para selecionar as opções desejadas na execução do programa, como o tamanho máximo e a visualização de estatísticas.

