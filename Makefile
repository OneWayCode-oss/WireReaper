# =====================================================================================
#                                     Makefile for HexGaze
# =====================================================================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = hexgaze
SRC = hexgaze.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

# Вспомогательная цель для быстрого запуска, требующая имя интерфейса через переменную IFACE
# Пример: make run IFACE=eth0
run: $(TARGET)
	@if [ -z "$(IFACE)" ]; then \
		echo "[!] Ошибка: Не указан сетевой интерфейс."; \
		echo "    Используйте: make run IFACE=<имя_интерфейса>"; \
		echo "    Пример:      make run IFACE=eth0"; \
		exit 1; \
	fi
	sudo ./$(TARGET) $(IFACE)
