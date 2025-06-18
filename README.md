
# Projeto de Geração, Ordenação e Consulta de Leituras de Sensores

Este projeto consiste em três programas escritos em C:

1. `gerar.c` - Gera dados de sensores.
2. `ordenar.c` - Ordena os dados por timestamp de forma decrescente.
3. `consultar.c` - Consulta o dado mais próximo a uma data e hora informada.

---

## 🧰 Compilação

```bash
gcc program1_gerar.c -o gerar
gcc program2_ordenar.c -o ordenar
gcc program3_consultar.c -o consultar
```

---

## ⚙️ Uso dos Programas

### 1. Gerar dados (`gerar`)

Gera leituras de sensores com base em um intervalo de tempo.

📌 Sintaxe:

```bash
./gerar <data_inicio> <hora_inicio> <data_fim> <hora_fim> <sensor1>:<tipo> <sensor2>:<tipo> ...
```

🗓️ Formato da data: `DD/MM/AAAA`  
🕐 Formato da hora: `HH:MM:SS`

🔢 Tipos de sensores disponíveis:

- `CONJ_Z` – Números inteiros
- `CONJ_Q` – Números decimais
- `TEXTO`  – Palavras aleatórias (ex: Norte, Sul, etc.)
- `BINARIO` – true ou false

✅ Exemplo:

```bash
./gerar 14/06/2025 15:00:00 15/06/2025 18:30:00 temperatura:CONJ_Q umidade:CONJ_Z aviso:TEXTO status:BINARIO
```

📦 Saída: `sensores.txt`

---

### 2. Ordenar dados (`ordenar`)

Ordena as leituras por timestamp em ordem decrescente e cria um arquivo separado por sensor.

📌 Sintaxe:

```bash
./ordenar sensores.txt
```

📁 Saídas:

- `decrescente.txt` — com todas as leituras ordenadas
- `temperatura.txt`, `umidade.txt`, etc. — um arquivo por sensor

---

### 3. Consultar leitura (`consultar`)

Busca a leitura mais próxima da data e hora informadas para um sensor específico.

📌 Sintaxe:

```bash
./consultar <sensor> <data> <hora>
```

🗓️ Formato da data: `DD/MM/AAAA`  
🕐 Formato da hora: `HH:MM:SS`

✅ Exemplo:

```bash
./consultar temperatura 14/06/2025 16:45:00
```

---

## 📝 Observações

- Os programas verificam erros de entrada como datas inválidas (ex: 31/02/2024 ou 25:61:99).
- O terminal deve suportar UTF-8 para exibir corretamente acentos e caracteres especiais.
- Cada sensor gerado com `TEXTO` recebe aleatoriamente um dos valores: `Norte`, `Sul`, `Leste`, `Oeste`, `Centro`.

---

## 🧪 Execução completa sugerida

```bash
./gerar 14/06/2025 15:00:00 15/06/2025 18:30:00 temperatura:CONJ_Q umidade:CONJ_Z aviso:TEXTO status:BINARIO
./ordenar sensores.txt
./consultar temperatura 14/06/2025 16:45:00
```

---

## 👤 Autor

Rodrigo — Projeto de sensores em linguagem C.
