# =========================================================================
# Makefile
# =========================================================================
# autor: Mateusz Kruk, grupa 7.
# utworzony: 3.04.2021
# modyfikowany ostatnio: 15.04.2021
# =========================================================================

# =========================================================================
# Makrodefinicje
# =========================================================================

# -------------------------------------------------------------------------
# Nazwa zestawu
# -------------------------------------------------------------------------

MODULE := Zestaw045
NAME := Mateusz
SURNAME := Kruk

# -------------------------------------------------------------------------
# Nazwy zadań
# -------------------------------------------------------------------------

ZAD1 := prodkons
ZAD2A := fifo
ZAD2B := producent
ZAD2C := konsument

# -------------------------------------------------------------------------
# Nazwy plików wykonywalnych
# -------------------------------------------------------------------------

EXEC1 := $(ZAD1).x
EXEC2A := $(ZAD2A).x
EXEC2B := $(ZAD2B).x
EXEC2C := $(ZAD2C).x

# -------------------------------------------------------------------------
# Makrodefinicje kompilacji i linkowania
# -------------------------------------------------------------------------

CC := gcc
CFLAGS := -Wall -std=c99 -pedantic
LDFLAGS := -s -lm
LDLIBS :=

# =========================================================================
# Reguły
# =========================================================================

# -------------------------------------------------------------------------
# Reguły nieodwołujące się do plików
# -------------------------------------------------------------------------

.PHONY: all archive clean clean_o 

all: $(EXEC1) $(EXEC2A) $(EXEC2B) $(EXEC2C) clean_o


# -------------------------------------------------------------------------
# Reguły wzorcowe
# -------------------------------------------------------------------------

%.x: %.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


# -------------------------------------------------------------------------
# Linkowanie prgramów z zadania 2
# -------------------------------------------------------------------------

$(ZAD2A).o : $(ZAD2A).c $(EXEC2B) $(EXEC2C)

# -------------------------------------------------------------------------
# Reguła archiwizująca cały Zestaw
# -------------------------------------------------------------------------

archive: clean
	(cd ../; tar -cvzf $(NAME)$(SURNAME)_$(MODULE).tar.gz  $(MODULE) )

# -------------------------------------------------------------------------
# Reguła czyszcząca pliki tymczasowe
# -------------------------------------------------------------------------

clean:
	$(RM) *.[ox]

clean_o:
	$(RM) *.o

# =========================================================================
