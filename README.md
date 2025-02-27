# Fasta Splitting Tool

## Overview
The **Fasta Splitting Tool** is a command-line utility designed to efficiently split FASTA files based on either:
- The number of FASTA entries
- The total nucleotide count

This tool is useful for processing large FASTA files by breaking them into smaller, more manageable chunks.

## Features
- Supports splitting FASTA files by a specified number of entries.
- Allows splitting based on a defined nucleotide count threshold.
- Outputs split files into organized directories with meaningful names.
- Efficiently processes large files while maintaining data integrity.

## Usage
```
./FastaSplittingTool -n <fasta_file> --fasta_entry_count <count>
./FastaSplittingTool -n <fasta_file> --nucleotides_count <count>
```
### Example:
To split a file `sequences.fasta` into chunks of 100 entries each:
```
./FastaSplittingTool -n sequences.fasta --fasta_entry_count 100
```
To split the same file into parts of 5000 nucleotides each:
```
./FastaSplittingTool -n sequences.fasta --nucleotides_count 5000
```

## Implementation Details
- Uses **C++17** and `std::filesystem` for file handling.
- Processes FASTA entries efficiently using `std::vector` and `std::string_view`.
- Writes output files in `.fasta` format with preserved data structure.
- Generates structured output directories based on the chosen splitting criteria.

## Requirements
- A C++17-compatible compiler
- Standard C++ libraries (no external dependencies)

## Compilation
Use `g++` to compile:
```
g++ -std=c++17 -o FastaSplittingTool main.cpp
```

## License
This project is released under the **MIT License**.
