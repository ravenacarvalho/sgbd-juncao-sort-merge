# Junção Sort-Merge (SGBD)

Trabalho 3 da disciplina CK0095 - Sistemas de Bancos de Dados (2026.1).
Implementação em C++ do operador de **junção Sort-Merge** entre as tabelas `Grapes`
(uvas) e `Wines` (vinhos), sem uso de SGBD. O operador usa **ordenação externa** e
depois executa o **Merge Join**, sem modificar as tabelas originais.

## Alunos

Ravena Carvalho e Luis Francisco

**Professor:** Javam Machado

---

## Esquema

| Tabela | Colunas | Chave |
|---|---|---|
| `Grapes` | `chave_primaria`, `grape_synonym`, `grape_country` | PK `chave_primaria` (coluna 0) |
| `Wines` | `vinho`, `chave_estrangeira`, `origem`, `tipo`, `estilo` | FK `chave_estrangeira` (coluna 1) |

A junção compara `Grapes.chave_primaria` com `Wines.chave_estrangeira`. Cada tupla do
resultado traz as colunas da uva seguidas das colunas do vinho.

## Estrutura do projeto

```
.
├── includes/
│   ├── tupla.hpp              # Tupla: vetor de colunas (strings)
│   ├── pagina.hpp             # Pagina: até 12 tuplas (MAX_TUPLAS)
│   ├── tabela.hpp             # Tabela: vetor de páginas
│   ├── esquema.hpp            # Esquema (nomes das colunas)
│   ├── carregamento.hpp       # leitura de CSV e impressão
│   ├── ordenacao_externa.hpp  # ordenação externa
│   └── merge_join.hpp         # Merge Join
├── src/
│   ├── main.cpp               # casos de teste e execução com os dados reais
│   ├── carregamento.cpp
│   ├── ordenacao_externa.cpp
│   └── merge_join.cpp
├── dados/
│   ├── grapes.csv
│   └── wines.csv
├── testes/casosdeteste.txt    # descrição dos casos de teste
├── Makefile
└── README.md
```

## Compilação

Requer `g++`.

```bash
make          # gera o executável ./main
make clean    # remove os objetos e o executável
```

## Execução

Execute a partir da **raiz do projeto**, pois os CSVs são lidos de `dados/`:

```bash
./main
```

O programa faz duas coisas, nesta ordem:

1. Roda os **5 casos de teste** e imprime `PASSOU` ou `FALHOU` para cada um.
2. Carrega `dados/grapes.csv` e `dados/wines.csv`, ordena as duas tabelas, executa o
   Merge Join e imprime as tuplas resultantes em formato CSV.

## Como funciona

### Organização dos dados

Cada tabela é um conjunto de páginas, e cada página guarda no máximo **12 tuplas**.
Ao carregar o CSV, uma nova página é aberta sempre que a última está cheia.

### 1. Ordenação externa (`ordenacaoExterna`)

Com buffer de **B = 5 frames**:

1. **Geração de runs:** a tabela é lida em blocos de até **5 páginas**. Cada bloco é
   ordenado em memória pela coluna de junção e vira uma *run* ordenada.
2. **Intercalação:** as runs são intercaladas em grupos de até **4** (4 frames de
   entrada). O resultado é montado em um buffer de saída de **1 página** (12 tuplas),
   que é gravado quando enche.
3. O passo 2 se repete, em novas passagens, até restar **uma única run**, que substitui
   a tabela ordenada.

### 2. Merge Join (`mergeJoin`)

Com as duas tabelas ordenadas, dois ponteiros (`i` e `j`) percorrem as tabelas:

- chaves iguais: gera **todas as combinações** entre as tuplas da esquerda e da direita
  com aquela chave (produto cartesiano do grupo) e avança os dois ponteiros além do grupo;
- chave da esquerda menor: avança `i`;
- chave da direita menor: avança `j`.

O resultado é uma nova tabela, também paginada em 12 tuplas.

> As chaves são comparadas como **strings** (ordem lexicográfica), tanto na ordenação
> quanto no join.

## Casos de teste

| Caso | Cenário | Esperado |
|---|---|---|
| 1 | Cada uva tem exatamente um vinho | 2 tuplas |
| 2 | Uma uva com dois vinhos | uva repetida para cada vinho |
| 3 | Nenhuma chave em comum | resultado vazio |
| 4 | Duas uvas e dois vinhos com a mesma chave | produto cartesiano (4 tuplas) |
| 5 | Ordenação externa isolada, sem join | tuplas ordenadas pela chave |

Os dados de cada caso estão descritos em `testes/casosdeteste.txt` e implementados em
`src/main.cpp`.

## Limitações conhecidas

- **Leitura de CSV:** o carregamento divide cada linha por vírgula e não trata campos
  entre aspas. Nomes de vinhos e sinônimos de uvas que contêm vírgula são divididos em
  mais colunas, o que desloca a coluna da chave estrangeira e faz o join perder tuplas
  nos dados reais.
- **Simulação de disco e buffer:** as runs e as tabelas ordenadas são mantidas em
  estruturas em memória. A fase de Merge Join converte as tabelas ordenadas em vetores,
  em vez de carregar páginas em frames de buffer uma a uma.