# Migrating Birds Optimization for the Job Shop Scheduling Problem (JSSP)

Build and compile:

```
cmake -B build && cmake --build build
```

Compile:
```
cmake --build build
```

Run test:
```
ctest --test-dir build
```

Run:
```
./build/jssp_runner -instance <path_to_instance> [options]
```

## Branches

* **`main`**: Current version of the solver. Includes the `-startRandom` flag to initialize solutions from completely random operation permutations instead of job-based feasible permutations.
* **`refactor`**: Alternative architectural state of the project (does not include the `-startRandom` flag).

## Execution Modes

The program supports 3 execution modes via the `-mode` flag:

1. **`single`**: Runs one experiment using the exact hyperparameters provided via CLI flags (or defaults if omitted).
2. **`seeds`**: Runs *N* experiments using the same hyperparameters provided via CLI flags (or defaults if omitted) with incremental seeds (`seed`, `seed + 1`, ...).
3. **`random`**: Runs *N* experiments with randomly generated valid MBO hyperparameters for parameter exploration. Typically used with `-nRuns`.

## CLI Flags

| Flag | Description | Default Value |
|------|-------------|---------------|
| `-instance` | **[Required]** Path to the JSSP instance file. | - |
| `-out` | Path to save the output CSV report. | `./data/output/<timestamp>_<label>.csv` |
| `-label` | Experiment label used in reports and output filenames. | `MBO` |
| `-mode` | Execution mode (`single`, `seeds`, `random`). | `single` |
| `-nRuns` | Number of executions (used with `seeds` or `random`). | `1` |
| `-seed` | Base RNG seed for reproducibility. | `1` |
| `-threads` | Number of worker threads (`0` = automatic hardware concurrency). | `0` |
| `-startRandom` | *(Only in `main`)* Initialize solutions with a pure random permutation instead of a job-ordered permutation. | `false` |
| `-n` | Number of birds in the flock (must be odd, $n \ge 3$). | `51` |
| `-k` | Number of neighbor solutions generated per bird ($k \ge 2x + 1$). | `5` |
| `-x` | Number of unused best neighbors shared with trailing birds. | `1` |
| `-t` | Number of tours (wing-flap iterations) before replacing the leader bird. | `1000` |
| `-K` | Maximum total number of objective function evaluations. | `1000000` |