
SHELL = /bin/sh

all: ISAV_Alpine.pdf

ISAV_Alpine.pdf: *.tex *.bib *.inc
	pdflatex ISAV_Alpine
	pdflatex ISAV_Alpine
	bibtex ISAV_Alpine
	touch ISAV_Alpine.tex
	pdflatex ISAV_Alpine
	pdflatex ISAV_Alpine


clean:
	if (rm -f *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.brf *.lbl *.gz ISAV_Alpine.pdf) then :; fi

