# Projeto: Agenda PBuffer

## Informações do Autor
**Nome:** Eduardo Machado Behling

**Curso:** Ciência da Computação

**Materia:** Algoritmos e Estruturas de Dados (M1)

**Profesor:** Rafael Piccin Torchelsen

## Descrição do Problema
Este projeto implementa uma agenda que permite adicionar, remover, buscar e listar entradas. O sistema foi desenvolvido utilizando C e segue as regras do desafio proposto.

### Desafio
O objetivo deste desafio é implementar uma agenda sem utilizar variáveis declaradas no programa. Em vez disso, todas as informações são armazenadas em um único buffer de memória (`void *pBuffer`), o que significa que o gerenciamento de memória deve ser feito manualmente.

#### Regras do desafio:
- Nenhuma variável pode ser declarada em todo o programa, apenas ponteiros.
- Todos os dados do programa devem ser armazenados dentro do `pBuffer`.
- Nem mesmo como parâmetro de função pode-se declarar variáveis comuns.
- Não pode usar `struct` em todo o programa.
- O programa deve implementar o seguinte menu:
  1. Adicionar Pessoa (Nome, Idade, Email)
  2. Remover Pessoa
  3. Buscar Pessoa
  4. Listar todos
  5. Sair

## Testes Realizados
- **Adicionar Entrada:** Funcionou corretamente, os dados foram armazenados no buffer e recuperados com sucesso.
- **Buscar Entrada:** Pesquisa realizada com diferentes tamanhos de nomes, funcionando corretamente.
- **Remover Entrada:** Entradas foram removidas corretamente e a estrutura permaneceu intacta.
- **Listar Entradas:** Foi possivel listar as entradas e a estrutura permaneceu intacta.
- **Valgrind:** Código testado e aprovado sem memory leaks.

## O que Funciona e o que Não Funciona
### ✅ Funcionalidades que estão OK
- Adicionar entrada.
- Remover entrada.
- Buscar entrada.
- Listar todas as entradas.
- Compila corretamente com CMake.
- Código passou no teste do Valgrind sem memory leaks.
- Validação de entradas (nomes, idades e emails)

### ❌ O que ainda precisa de melhoria
- Tratamento de erros: tratar erros de *edge cases*, como falha na allocação, falha em funções do `stdio.h` e etc.
- Testes de estresse: ainda não foi testado com um grande volume de dados.

## Como Compilar e Executar
1. Certifique-se de ter o CMake instalado.
2. No terminal, navegue até a pasta do projeto e execute os seguintes comandos:
   ```sh
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ./bin/agenda
   ```
