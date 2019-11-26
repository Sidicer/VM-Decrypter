#include <iostream>
#include <fstream>

struct komanda {
  char kodas;
  char comp;
};

int main() {
  std::fstream decryptor;
  decryptor.open("decryptor.bin", std::ios::in|std::ios::binary);
  std::ifstream input_file("q1_encr.txt");
  std::ofstream output_file("output.txt");

  unsigned char regs[16];
  char prog_memory[256];
  bool endOfFile = false;

  for (int i = 0; i < 256; i++) {
    decryptor >> std::noskipws >> prog_memory[i];
    if (decryptor.eof())
      break;
  }
  decryptor.close();

  komanda* vm;
  vm = (komanda*)prog_memory;

  int pirmas_registras, antras_registras;
  bool vm_work = true;

  while (vm_work) {
    pirmas_registras = vm->comp & 0x0F;
    antras_registras = (vm->comp & 0xF0) >> 4;

    switch (vm->kodas) {
      case 0x10:
        input_file >> regs[pirmas_registras];
        if (input_file.eof())
          endOfFile = true;
      break;
      case 0x11:
        output_file << (char)regs[pirmas_registras];
      break;
      case 0x01:
        regs[pirmas_registras]++;
      break;
      case 0x02:
        regs[pirmas_registras]--;
      break;
      case 0x03:
        regs[pirmas_registras] = regs[antras_registras];
      break;
      case 0x04:
        regs[pirmas_registras] = vm->comp;
      break;
      case 0x05:
        regs[pirmas_registras] = regs[pirmas_registras] << 1;
      break;
      case 0x06:
        regs[pirmas_registras] = regs[pirmas_registras] >> 1;
      break;
      case 0x07:
        vm = (komanda*) ( (char*) vm + vm->comp );
        vm--;
      break;
      case 0x08:
      break;
      case 0x09:
      break;
      case 0x0A:
        if (endOfFile) {
          vm = (komanda*) ( (char*) vm + vm->comp );
          vm--;
        }
      break;
      case 0x0B:
        input_file.close();
        vm_work = false;
      break;
      case 0x0C:
        regs[pirmas_registras] = regs[pirmas_registras] + regs[antras_registras];
      break;
      case 0x0D:
        regs[pirmas_registras] = regs[pirmas_registras] - regs[antras_registras];
      break;
      case 0x0E:
        regs[pirmas_registras] = regs[pirmas_registras] ^ regs[antras_registras];
      break;
      case 0x0F:
        regs[pirmas_registras] = regs[pirmas_registras] | regs[antras_registras];
      break;
    }
    vm++;
  }

  return 0;
}
