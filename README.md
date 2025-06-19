
# Atividade 1 - N2 

Este projeto consiste em três programas escritos em C:

1. `program1_generate.c` - Gera dados de sensores.
2. `program2_sort.c` - Ordena os dados por timestamp de forma decrescente.
3. `program3_check.c` - Consulta o dado mais próximo a uma data e hora informada.

---

## 🧰 Compilação

```bash
gcc program1_generate.c -o generate
gcc program2_sort.c -o sort
gcc program3_check.c -o check
```

---

## ⚙️ Uso dos Programas

### 1. Gerar dados (`generate`)

Gera leituras de sensores com base em um intervalo de tempo, podendo gerar um sensor com nome desejado.

📌 Sintaxe:

```bash
./generate data e hora de inicio, data e hora final, primeiro sensor: TIPO, segundo sensor: TIPO ...
```

🗓️ Formato da data: `dia/mes/ano`  
🕐 Formato da hora: `HH:MM:SS`

🔢 Tipos de sensores disponíveis:

- `CONJ_Z` – Números inteiros
- `CONJ_Q` – Números decimais
- `TEXTO`  – String
- `BINARIO` – true ou false

✅ Exemplo:

```bash
./generate 14/06/2025 15:00:00 15/06/2025 18:30:00 temperature:CONJ_Q humidity:CONJ_Z warning:TEXTO status:BINARIO
```

📦 Saída: `sensors.txt`

---

### 2. Ordenar dados (`sort`)

Ordena as leituras por timestamp em ordem decrescente e cria um arquivo separado por sensor.

📌 Sintaxe:

```bash
./sort sensors.txt
```

📁 Saídas:

- `decrescente.txt` — com todas as leituras organizadas em ordem decrescente
- `temperature.txt`, `humidity.txt`, etc. — um arquivo por sensor

---

### 3. Consultar leitura (`check`)

Busca a leitura mais próxima da data e hora informadas para um sensor específico.

📌 Sintaxe:

```bash
./check sensor, data e hora
```

🗓️ Formato da data: `DD/MM/AAAA`  
🕐 Formato da hora: `HH:MM:SS`

✅ Exemplo:

```bash
./check temperature 14/06/2025 16:45:00
```

---

## 📝 Observações

- Os sensores podem ser gerados com nome digitado antes do tipo escolhido
- Os programas verificam erros de entrada como datas inválidas (ex: 31/02/2024 ou 25:61:99).
- Cada sensor gerado com `TEXTO` recebe aleatoriamente um dos valores: `Norte`, `Sul`, `Leste`, `Oeste`, `Centro`.

---

## 🧪 Execução completa sugerida

```bash
./generate 14/06/2025 15:00:00 15/06/2025 18:30:00 temperature:CONJ_Q humidity:CONJ_Z warning:TEXTO status:BINARIO
./sort sensores.txt
./check temperature 14/06/2025 16:45:00
``` 

---

## 👤 Autor

Rodrigo Júnio Oliveira — Atividade 1 Individual - N2.
