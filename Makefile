CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99
TARGET  = simulador
SRCS    = main.c arquivo.c memoria_ops.c visualizacao.c

all: $(TARGET)
	@$(MAKE) --no-print-directory help

$(TARGET): $(SRCS) memoria.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

first: $(TARGET)
	./$(TARGET) entrada_first_fit.txt

best: $(TARGET)
	./$(TARGET) entrada_best_fit.txt

worst: $(TARGET)
	./$(TARGET) entrada_worst_fit.txt

comparativo: $(TARGET)
	@echo ""
	@echo "========== FIRST FIT =========="
	@sed 's/^# Algoritmo:.*/# Algoritmo: FIRST_FIT/' entrada_comparativo.txt > /tmp/comp_ff.txt
	@./$(TARGET) /tmp/comp_ff.txt
	@echo ""
	@echo "========== BEST FIT ==========="
	@sed 's/^# Algoritmo:.*/# Algoritmo: BEST_FIT/' entrada_comparativo.txt > /tmp/comp_bf.txt
	@./$(TARGET) /tmp/comp_bf.txt
	@echo ""
	@echo "========== WORST FIT =========="
	@sed 's/^# Algoritmo:.*/# Algoritmo: WORST_FIT/' entrada_comparativo.txt > /tmp/comp_wf.txt
	@./$(TARGET) /tmp/comp_wf.txt

clean:
	rm -f $(TARGET)

help:
	@echo "Simulador de Alocacao de Memoria — UFMS/FACOM SO 2026.1"
	@echo ""
	@echo "Uso: ./simulador <arquivo.txt> [-a ALGORITMO]"
	@echo ""
	@echo "Algoritmos: FIRST_FIT, BEST_FIT, WORST_FIT"
	@echo ""
	@echo "Targets:"
	@echo "  make            Compila o simulador"
	@echo "  make first      Executa com First Fit"
	@echo "  make best       Executa com Best Fit"
	@echo "  make worst      Executa com Worst Fit"
	@echo "  make comparativo  Executa os 3 algoritmos na mesma entrada"
	@echo "  make clean      Remove o binario"
	@echo "  make help       Exibe esta ajuda"

.PHONY: all clean first best worst comparativo help
