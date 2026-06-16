# Compilacao do relatorio LaTeX (biblatex + biber)
# O PDF final e copiado para a raiz do projeto, que e o que vai na entrega.

TEX     = relatorio.tex
PDF     = relatorio.pdf
DESTINO = ../relatorio.pdf

all: $(PDF)
	cp $(PDF) $(DESTINO)
	@echo "PDF gerado e copiado para a raiz: $(DESTINO)"

$(PDF): $(TEX) bibliography.bib
	latexmk -pdf -interaction=nonstopmode $(TEX)

clean:
	latexmk -C
	rm -f *.bbl *.bcf *.run.xml

.PHONY: all clean
