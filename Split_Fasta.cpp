#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

class Fasta_Entry {
   public:
    Fasta_Entry() = delete;

    Fasta_Entry(const std::string_view title, const std::string_view sequence)
        : title_(title), sequence_(sequence) {}

    void PrintEntry(std::ostream& out) const { out << title_ << '\n' << sequence_; }

    size_t TotalSize() const { return title_.size() + sequence_.size(); }

    size_t SequenceSize() const { return sequence_.size(); }

   private:
    std::string_view title_;
    std::string_view sequence_;
};

std::ostream& operator<<(std::ostream& out, const Fasta_Entry& e) {
    e.PrintEntry(out);
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    bool if_first = true;
    for (const auto& item : v) {
        if (if_first) {
            out << item;
            if_first = false;
        } else {
            out << '\n' << item;
        }
    }
    return out;
}

std::vector<Fasta_Entry> SplitString(const std::string& s) {
    std::vector<Fasta_Entry> v;
    size_t index_title_start = 0;
    size_t index_sequence_end = 0;
    size_t index_title_end = 0;
    size_t index_sequence_start = 0;
    std::string_view title;
    std::string_view sequence;

    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\n') {
            index_title_end = i;
            title = std::string_view(s.data() + index_title_start, index_title_end - index_title_start);
            index_sequence_start = index_title_end + 1;
        } else if (i != 0 && (s[i] == '>' || i == s.size() - 1)) {
            index_sequence_end = (s[i] == '>' ? i : i + 1);
            index_title_start = i;
            sequence =
                std::string_view(s.data() + index_sequence_start, index_sequence_end - index_sequence_start);
            v.emplace_back(title, sequence);
        }
    }
    return v;
}

template <typename It>
void PrintPart(It start, It end, std::ostream& out) {
    bool first = true;
    while (start != end) {
        if (first) {
            out << *start;
            first = false;
        } else {
            out << '\n' << *start;
        }
        ++start;
    }
}

void SplitByItemCount(const std::vector<Fasta_Entry>& v, int count, const std::string& prefix) {
    size_t start = 0;
    int counter = 0;
    int file_name = 1;
    std::string dir_name = prefix + "_Splitted_by_Item_Count = " + std::to_string(count);
    std::filesystem::create_directory(std::filesystem::path(dir_name));
    for (size_t i = 0; i < v.size(); ++i) {
        ++counter;
        if (counter == count) {
            std::string name_s(dir_name + '/' + std::to_string(file_name++));
            std::ofstream out(name_s + ".fasta");
            PrintPart(v.begin() + start, v.begin() + start + counter, out);
            start += counter;
            // std::cout << start << " " << counter << std::endl;
            counter = 0;
        }
    }

    if (v.begin() + start != v.end()) {
        std::string name_s(dir_name + '/' + std::to_string(file_name));
        std::ofstream out(name_s + ".fasta");
        PrintPart(v.begin() + start, v.end(), out);
    }
}

void SplitByNucleotidesCount(const std::vector<Fasta_Entry>& v, int nucleotide_count,
                             const std::string& prefix) {
    size_t start = 0;
    int counter = 0;
    int file_name = 1;
    std::string dir_name =
        prefix + "_Files_Splitted_by_Nucleotide_Count = " + std::to_string(nucleotide_count);
    std::filesystem::create_directory(std::filesystem::path(dir_name));
    for (size_t i = 0; i < v.size(); ++i) {
        counter += v[i].TotalSize();
        if (counter > nucleotide_count) {
            std::string name_s(dir_name + '/' + std::to_string(file_name++));
            std::ofstream out(name_s + ".fasta");
            PrintPart(v.begin() + start, v.begin() + i, out);
            start = i;
            counter = 0;
            counter += v[i].SequenceSize();
        }
    }

    if (v.begin() + start != v.end()) {
        std::string name_s(dir_name + '/' + std::to_string(file_name));
        std::ofstream out(name_s + ".fasta");
        PrintPart(v.begin() + start, v.end(), out);
    }
}

void PrintHelp() {
    std::cout << "Usage:\n";
    std::cout << "-n \"filename\" (path) --fasta_entry_count (integer)\n";
    std::cout << "-n \"filename\" (path) --nucleotides_count (integer)\n";
}

std::string ReadFile(std::fstream& file) {
    std::string s;
    std::string tmp;

    while (std::getline(file, tmp)) {
        if (tmp[0] == '>') {
            s += tmp;
            s += '\n';
        } else {
            s += tmp;
        }
        tmp.clear();
    }
    return s;
}

int main(int argc, char* argv[]) {
    if (argc < 5 || std::string(argv[1]) == "-h" || std::string(argv[2]) == "--help" ||
        std::string(argv[1]) != "-n") {
        PrintHelp();
    } else {
        std::fstream file(argv[2]);
        std::string s = ReadFile(file);
        std::vector<Fasta_Entry> v = SplitString(s);
        if (std::string(argv[3]) == "--fasta_entry_count") {
            SplitByItemCount(v, std::atoi(argv[4]), std::string(argv[2]));
        } else if (std::string(argv[3]) == "--nucleotides_count") {
            SplitByNucleotidesCount(v, std::atoi(argv[4]), std::string(argv[2]));
        } else {
            PrintHelp();
        }
    }

    return 0;
}
