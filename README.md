# Projeto de Sensores em C

Este projeto possui três programas principais para trabalhar com dados simulados de sensores:

- 📄 program3_gerar.c — Gera dados simulados.
- 📄 program1_ordenar.c — Ordena os dados por timestamp e separa por sensor.
- 📄 program2_consultar.c — Consulta uma leitura específica por sensor e data/hora.

---

## 🔧 Compilação

Use o gcc para compilar cada programa:

```bash
gcc program1_gerar.c -o gerar
gcc program2_ordenar.c -o ordenar
gcc program3_consultar.c -o consultar
```

---

## 📊 1. Gerar dados simulados

Gera um arquivo `dados_teste.txt` com leituras aleatórias para cada sensor.

🧾 Uso:

```bash
./gerar <timestamp_inicio> <timestamp_fim> <sensor1>:<TIPO> <sensor2>:<TIPO> ...
```

🧪 Tipos de sensores válidos:

- CONJ_Z → Inteiro
- CONJ_Q → Número decimal
- TEXTO  → Palavra (ex: "Alfa", "Beta", etc.)
- BINARIO → true ou false

✅ Exemplo:

```bash
./gerar 18/06/2024 15:00:00 18/06/2025 15:00:00 temperatura:CONJ_Q umidade:CONJ_Z status:BINARIO aviso:TEXTO

./gerar 18/06/2024 15:00:00 18/06/2025 15:00:00 temperatura: umidade: status: aviso: //CERTO

./gerar 18/06/2024 15:00:00 18/06/2025 15:00:00 CONJ_Q CONJ_Z BINARIO TEXTO //CERTO

./gerar temperatura:CONJ_Q umidade:CONJ_Z status:BINARIO aviso:TEXTO //CERTO
```

Isso criará 2000 leituras para cada sensor entre os timestamps informados.

---

## 📑 2. Ordenar e separar sensores

Lê o arquivo `sensores.txt`, ordena por timestamp decrescente e:

- Salva tudo em `ordenado.txt`
- Cria um arquivo separado para cada sensor (ex: temperatura.txt, umidade.txt)

🧾 Uso:

```bash
./ordenar sensores.txt

./ordenar arq_invalido.txt
```

---

## 🔍 3. Consultar por sensor e data/hora

Consulta a leitura mais próxima de uma data e hora para um sensor específico.

🧾 Uso:

```bash
./consultar <sensor> <AAAA-MM-DD> <HH:MM:SS>
```

✅ Exemplo:

```bash
./consultar temperatura 18/06/2024 15:00:00
./consultar status 18/06/2024 15:00:00
./consultar umidade 18/06/2024 15:00:00
./consultar aviso 18/06/2024 15:00:00
```

Isso procura em temperatura.txt a leitura mais próxima de 16 de junho de 2025, às 14:00:00.

---

## 📌 Observações

- Os arquivos separados dos sensores são sobrescritos a cada execução de ordenação.
- O timestamp é baseado no formato Unix (segundos desde 01/01/1970).
- Certifique-se de rodar os comandos na ordem: gerar → ordenar → consultar.

---

🛠️ Feito com 💻 em linguagem C

