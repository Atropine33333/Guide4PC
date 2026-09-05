# Section 7.9: Modifying the Blackjack Program to Log Game Data

## 7.9.1 Why Log Data

The Blackjack program you wrote generates data every time it runs – one or more rounds of gameplay. But that data disappears as soon as the program closes. If you want to accumulate data for analysis – for example, to calculate the player's win rate, average hand totals, bust probabilities, or the effectiveness of different strategies – you need to persist that data to a file.

The goal of this section is to modify the Blackjack program to write key round‑by‑round data to a `BlackJack.csv` file.

## 7.9.2 What Is CSV

CSV (Comma‑Separated Values) is the simplest tabular data format. Each line is a row of data, and different fields are separated by commas.

For example:

```
player_total,dealer_total,result,player_initial,player_final
20,18,win,A 10,A 10
15,22,lose,2 5,2 5 7 3
```

The first line is the header (column names), and subsequent lines contain the actual data.

CSV files can be opened with any text editor, viewed directly in Excel or VS Code, and read/written using Python's built‑in `csv` module. In later sections, you'll learn how to analyse this file with `pandas`.

## 7.9.3 What Data to Log

To analyse the game, you need to record enough information without logging every single detail. Here are the recommended fields:

| Field | Type | Description |
|--------|------|-------------|
| `timestamp` | str | Timestamp when the round ended |
| `player_total` | int | Player's final hand total |
| `dealer_total` | int | Dealer's final hand total |
| `player_bust` | bool | Whether the player busted |
| `dealer_bust` | bool | Whether the dealer busted |
| `player_blackjack` | bool | Whether the player had a natural 21 |
| `player_hit_count` | int | Number of times the player hit |
| `dealer_hit_count` | int | Number of times the dealer hit |
| `initial_bet` | int | Bet placed for this round |
| `final_chips` | int | Chip count after the round |
| `profit` | int | Profit for this round (positive = win, negative = loss) |
| `dealer_hole_card` | str | Dealer's hole card (for analysis) |
| `player_initial_cards` | str | Player's first two cards |
| `player_final_cards` | str | Player's final hand |

These fields give you plenty to analyse: at what totals players stand, how bust probability relates to starting hands, how the dealer's hole card affects outcomes, and how betting strategies correlate with profit.

## 7.9.4 Three Versions of the Program

This section provides three versions of the Blackjack program, with increasing complexity:

| Version | Filename | Purpose |
|---------|----------|---------|
| **Version 1** | `blackjack_original.py` | Original version – just plays the game, no logging |
| **Version 2** | `blackjack_csv.py` | Logs each round to CSV – good for manually accumulating data |
| **Version 3** | `blackjack_simulator.py` | Parallel simulation of large numbers of rounds, generating strategy comparison data |

All three versions share the same data format, so data from manual play and simulated runs can be combined for later analysis.

## 7.9.5 Version 1: Original (No Logging)

```python
#!/usr/bin/env python3
"""Blackjack (21) card game - Original version without logging."""

import random


def create_deck() -> list[tuple[str, int]]:
    ranks = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']
    values = {
        '2': 2, '3': 3, '4': 4, '5': 5, '6': 6,
        '7': 7, '8': 8, '9': 9, '10': 10,
        'J': 10, 'Q': 10, 'K': 10, 'A': 11
    }
    deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]
    random.shuffle(deck)
    return deck


def calculate_hand(hand: list[tuple[str, int]]) -> int:
    total = sum(value for _, value in hand)
    ace_count = sum(1 for rank, _ in hand if rank == 'A')
    while total > 21 and ace_count > 0:
        total -= 10
        ace_count -= 1
    return total


def display_hand(hand: list[tuple[str, int]], hide_first: bool = False) -> str:
    if hide_first:
        return f"[HIDDEN] {' '.join(f'{rank}' for rank, _ in hand[1:])}"
    return ' '.join(f'{rank}' for rank, _ in hand)


def dealer_should_hit(hand: list[tuple[str, int]]) -> bool:
    return calculate_hand(hand) < 17


def play_blackjack() -> None:
    print("=" * 50)
    print("BLACKJACK (21)")
    print("=" * 50)
    print("Commands: hit, stand, quit\n")

    chips = 100

    while True:
        print(f"\nYou have {chips} chips.")
        while True:
            bet_input = input("Place your bet (or type 'quit' to exit): ").strip().lower()
            if bet_input == 'quit':
                print("Thanks for playing!")
                return
            try:
                bet = int(bet_input)
                if bet <= 0:
                    print("Bet must be positive.")
                    continue
                if bet > chips:
                    print(f"You only have {chips} chips.")
                    continue
                break
            except ValueError:
                print("Invalid input. Enter a number.")

        chips -= bet

        deck = create_deck()
        player_hand = [deck.pop(), deck.pop()]
        dealer_hand = [deck.pop(), deck.pop()]

        print(f"\nDealer shows: {display_hand(dealer_hand, hide_first=True)}")
        print(f"Your hand: {display_hand(player_hand)} (total: {calculate_hand(player_hand)})")

        player_bust = False
        while True:
            player_total = calculate_hand(player_hand)
            if player_total > 21:
                player_bust = True
                break
            if player_total == 21:
                print("You got 21!")
                break

            action = input("Hit or stand? ").strip().lower()
            if action == 'quit':
                print("Thanks for playing!")
                return
            elif action == 'hit':
                new_card = deck.pop()
                player_hand.append(new_card)
                print(f"You drew: {new_card[0]}")
                print(f"Your hand: {display_hand(player_hand)} (total: {calculate_hand(player_hand)})")
            elif action == 'stand':
                break
            else:
                print("Invalid command.")
                continue

        dealer_bust = False
        if not player_bust:
            print(f"\nDealer's hand: {display_hand(dealer_hand)} (total: {calculate_hand(dealer_hand)})")
            while dealer_should_hit(dealer_hand):
                new_card = deck.pop()
                dealer_hand.append(new_card)
                print(f"Dealer drew: {new_card[0]}")
                print(f"Dealer's hand: {display_hand(dealer_hand)} (total: {calculate_hand(dealer_hand)})")

            dealer_total = calculate_hand(dealer_hand)
            if dealer_total > 21:
                dealer_bust = True

        player_total = calculate_hand(player_hand)
        dealer_total = calculate_hand(dealer_hand)

        print("\n" + "-" * 40)

        if player_bust:
            print("You busted! Dealer wins.")
        elif dealer_bust:
            print("Dealer busted! You win!")
            chips += bet * 2
        elif player_total > dealer_total:
            print(f"You win! {player_total} vs {dealer_total}")
            chips += bet * 2
        elif player_total < dealer_total:
            print(f"Dealer wins! {player_total} vs {dealer_total}")
        else:
            print(f"Push! {player_total} vs {dealer_total}")
            chips += bet

        print(f"Your chips: {chips}")

        if chips <= 0:
            print("\nYou're out of chips! Game over.")
            break


if __name__ == "__main__":
    play_blackjack()
```

## 7.9.6 Version 2: Interactive Game + CSV Logging

This version adds logging to the original game. After each round, key data is appended to `BlackJack.csv`.

```python
#!/usr/bin/env python3
"""Blackjack (21) card game with CSV logging.

Each round's data is logged to BlackJack.csv for later analysis.
"""

import random
import csv
from datetime import datetime
from pathlib import Path


def create_deck() -> list[tuple[str, int]]:
    ranks = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']
    values = {
        '2': 2, '3': 3, '4': 4, '5': 5, '6': 6,
        '7': 7, '8': 8, '9': 9, '10': 10,
        'J': 10, 'Q': 10, 'K': 10, 'A': 11
    }
    deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]
    random.shuffle(deck)
    return deck


def calculate_hand(hand: list[tuple[str, int]]) -> int:
    total = sum(value for _, value in hand)
    ace_count = sum(1 for rank, _ in hand if rank == 'A')
    while total > 21 and ace_count > 0:
        total -= 10
        ace_count -= 1
    return total


def display_hand(hand: list[tuple[str, int]], hide_first: bool = False) -> str:
    if hide_first:
        return f"[HIDDEN] {' '.join(f'{rank}' for rank, _ in hand[1:])}"
    return ' '.join(f'{rank}' for rank, _ in hand)


def dealer_should_hit(hand: list[tuple[str, int]]) -> bool:
    return calculate_hand(hand) < 17


def get_csv_path() -> Path:
    return Path(__file__).parent / "BlackJack.csv"


def init_csv_file() -> None:
    csv_path = get_csv_path()
    if csv_path.exists():
        return

    headers = [
        "timestamp", "player_total", "dealer_total",
        "player_bust", "dealer_bust", "player_blackjack",
        "player_hit_count", "dealer_hit_count",
        "initial_bet", "final_chips", "profit",
        "dealer_hole_card", "player_initial", "player_final"
    ]

    with open(csv_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(headers)

    print(f"[Log] Created CSV file: {csv_path}")


def log_round_data(round_data: dict) -> None:
    csv_path = get_csv_path()
    field_order = [
        "timestamp", "player_total", "dealer_total",
        "player_bust", "dealer_bust", "player_blackjack",
        "player_hit_count", "dealer_hit_count",
        "initial_bet", "final_chips", "profit",
        "dealer_hole_card", "player_initial", "player_final"
    ]

    row = [round_data.get(field, "") for field in field_order]

    with open(csv_path, 'a', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(row)


def play_blackjack() -> None:
    init_csv_file()

    print("=" * 50)
    print("BLACKJACK (21)")
    print("=" * 50)
    print("Commands: hit, stand, quit\n")

    chips = 100
    round_count = 0

    while True:
        print(f"\nYou have {chips} chips.")
        while True:
            bet_input = input("Place your bet (or type 'quit' to exit): ").strip().lower()
            if bet_input == 'quit':
                print("Thanks for playing!")
                return
            try:
                bet = int(bet_input)
                if bet <= 0:
                    print("Bet must be positive.")
                    continue
                if bet > chips:
                    print(f"You only have {chips} chips.")
                    continue
                break
            except ValueError:
                print("Invalid input. Enter a number.")

        chips_before_round = chips
        chips -= bet

        deck = create_deck()
        player_hand = [deck.pop(), deck.pop()]
        dealer_hand = [deck.pop(), deck.pop()]

        player_initial_cards = " ".join(rank for rank, _ in player_hand)
        dealer_hole = dealer_hand[0][0]

        print(f"\nDealer shows: {display_hand(dealer_hand, hide_first=True)}")
        print(f"Your hand: {display_hand(player_hand)} (total: {calculate_hand(player_hand)})")

        player_bust = False
        player_hit_count = 0
        player_blackjack = False

        while True:
            player_total = calculate_hand(player_hand)
            if player_total > 21:
                player_bust = True
                break
            if player_total == 21:
                if len(player_hand) == 2:
                    player_blackjack = True
                print("You got 21!")
                break

            action = input("Hit or stand? ").strip().lower()
            if action == 'quit':
                print("Thanks for playing!")
                return
            elif action == 'hit':
                new_card = deck.pop()
                player_hand.append(new_card)
                player_hit_count += 1
                print(f"You drew: {new_card[0]}")
                print(f"Your hand: {display_hand(player_hand)} (total: {calculate_hand(player_hand)})")
            elif action == 'stand':
                break
            else:
                print("Invalid command.")
                continue

        player_final_cards = " ".join(rank for rank, _ in player_hand)

        dealer_bust = False
        dealer_hit_count = 0

        if not player_bust:
            print(f"\nDealer's hand: {display_hand(dealer_hand)} (total: {calculate_hand(dealer_hand)})")
            while dealer_should_hit(dealer_hand):
                new_card = deck.pop()
                dealer_hand.append(new_card)
                dealer_hit_count += 1
                print(f"Dealer drew: {new_card[0]}")
                print(f"Dealer's hand: {display_hand(dealer_hand)} (total: {calculate_hand(dealer_hand)})")

            dealer_total = calculate_hand(dealer_hand)
            if dealer_total > 21:
                dealer_bust = True

        player_total = calculate_hand(player_hand)
        dealer_total = calculate_hand(dealer_hand)
        profit = 0

        print("\n" + "-" * 40)

        if player_bust:
            print("You busted! Dealer wins.")
            profit = -bet
        elif dealer_bust:
            print("Dealer busted! You win!")
            chips += bet * 2
            profit = bet
        elif player_total > dealer_total:
            print(f"You win! {player_total} vs {dealer_total}")
            chips += bet * 2
            profit = bet
        elif player_total < dealer_total:
            print(f"Dealer wins! {player_total} vs {dealer_total}")
            profit = -bet
        else:
            print(f"Push! {player_total} vs {dealer_total}")
            chips += bet
            profit = 0

        print(f"Your chips: {chips}")

        round_count += 1
        round_data = {
            "timestamp": datetime.now().isoformat(),
            "player_total": player_total,
            "dealer_total": dealer_total,
            "player_bust": player_bust,
            "dealer_bust": dealer_bust,
            "player_blackjack": player_blackjack,
            "player_hit_count": player_hit_count,
            "dealer_hit_count": dealer_hit_count,
            "initial_bet": bet,
            "final_chips": chips,
            "profit": profit,
            "dealer_hole_card": dealer_hole,
            "player_initial": player_initial_cards,
            "player_final": player_final_cards
        }
        log_round_data(round_data)
        print(f"[Log] Round {round_count} saved to BlackJack.csv")

        if chips <= 0:
            print("\nYou're out of chips! Game over.")
            break


if __name__ == "__main__":
    play_blackjack()
```

## 7.9.7 Version 3: Parallel Simulator

This version is designed for large‑scale simulation without human interaction. It uses `multiprocessing` to run many rounds in parallel, generating 10,000 rounds for each of three stand strategies (stand at 16, 17, and 18). It outputs three CSV files automatically for later analysis.

**Data format differences from Version 2**:

| Field | Version 2 | Version 3 | Notes |
|-------|-----------|-----------|-------|
| `timestamp` | ✅ | ❌ | Simulation data does not need timestamps |
| `initial_bet` | ✅ | ❌ | Simulation uses a fixed 1‑unit bet |
| `final_chips` | ✅ | ❌ | Replaced by cumulative `profit` |
| `strategy` | ❌ | ✅ | Records the stand threshold (16/17/18) |
| `round_id` | ❌ | ✅ | Sequential ID for simulation rounds |

**How to modify strategies**:

To test different stand thresholds, simply modify the `strategies` list:

```python
# Test other strategies
strategies = [15, 16, 17, 18, 19, 20]

# Or just two more aggressive strategies
strategies = [13, 17]

# Custom combination
strategies = [12, 14, 16, 18, 20]
```

To change the number of rounds per strategy:

```python
# Simulate 50,000 rounds per strategy (takes longer)
num_rounds = 50000

# Quick test with 100 rounds
num_rounds = 100
```

To adjust the number of parallel processes:

```python
# Use 8 processes (defaults to CPU count)
results = run_simulation(threshold, num_rounds, num_workers=8)
```

```python
#!/usr/bin/env python3
"""Parallel Blackjack Simulation.

Simulates 10,000 rounds for each of three player strategies:
- Stand at 16
- Stand at 17
- Stand at 18

Results are saved to three CSV files for subsequent analysis.
Uses multiprocessing to parallelize the simulation.
"""

import csv
import multiprocessing as mp
import random
import time
from pathlib import Path


# ============================================================================
# Game Core Functions
# ============================================================================

def create_deck() -> list[tuple[str, int]]:
    """Create a standard deck of 52 cards."""
    ranks = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']
    values = {
        '2': 2, '3': 3, '4': 4, '5': 5, '6': 6,
        '7': 7, '8': 8, '9': 9, '10': 10,
        'J': 10, 'Q': 10, 'K': 10, 'A': 11
    }
    deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]
    random.shuffle(deck)
    return deck


def calculate_hand(hand: list[tuple[str, int]]) -> int:
    """Calculate the total value of a hand."""
    total = sum(value for _, value in hand)
    ace_count = sum(1 for rank, _ in hand if rank == 'A')

    while total > 21 and ace_count > 0:
        total -= 10
        ace_count -= 1

    return total


def dealer_should_hit(hand: list[tuple[str, int]]) -> bool:
    """Determine if dealer should take another card."""
    return calculate_hand(hand) < 17


# ============================================================================
# Single Round Simulation
# ============================================================================

def play_round(threshold: int, round_id: int, base_seed: int) -> dict:
    """Play one round with the given stand threshold.

    Args:
        threshold: Player stands when hand total >= threshold.
        round_id: Unique round number (for logging).
        base_seed: Base seed value. Actual seed = base_seed + round_id.

    Returns:
        dict: Round statistics including initial and final hands.
    """
    # Set deterministic seed for reproducibility
    seed = base_seed + round_id
    random.seed(seed)

    # Create and shuffle deck
    deck = create_deck()

    # Deal initial cards
    player_hand = [deck.pop(), deck.pop()]
    dealer_hand = [deck.pop(), deck.pop()]

    # Record initial hand
    player_initial = [rank for rank, _ in player_hand]

    # Check for player blackjack
    player_blackjack = False
    if calculate_hand(player_hand) == 21 and len(player_hand) == 2:
        player_blackjack = True

    # ---- Player's turn ----
    player_bust = False
    player_hit_count = 0

    while True:
        player_total = calculate_hand(player_hand)

        if player_total > 21:
            player_bust = True
            break

        # Player stands if total >= threshold
        if player_total >= threshold:
            break

        # Hit: draw a card
        player_hand.append(deck.pop())
        player_hit_count += 1

    # Record final hand
    player_final = [rank for rank, _ in player_hand]

    # ---- Dealer's turn (only if player didn't bust) ----
    dealer_bust = False
    dealer_hit_count = 0

    if not player_bust:
        while dealer_should_hit(dealer_hand):
            dealer_hand.append(deck.pop())
            dealer_hit_count += 1

        dealer_total = calculate_hand(dealer_hand)
        if dealer_total > 21:
            dealer_bust = True
    else:
        dealer_total = calculate_hand(dealer_hand)

    # ---- Determine result ----
    player_total = calculate_hand(player_hand)

    if player_bust:
        result = "lose"
        profit = -1
    elif dealer_bust:
        result = "win"
        profit = 1
    elif player_total > dealer_total:
        result = "win"
        profit = 1
    elif player_total < dealer_total:
        result = "lose"
        profit = -1
    else:
        result = "push"
        profit = 0

    return {
        "round_id": round_id,
        "strategy": threshold,
        "player_total": player_total,
        "dealer_total": dealer_total,
        "player_bust": player_bust,
        "dealer_bust": dealer_bust,
        "player_blackjack": player_blackjack,
        "player_hit_count": player_hit_count,
        "dealer_hit_count": dealer_hit_count,
        "result": result,
        "profit": profit,
        "player_initial": " ".join(player_initial),
        "player_final": " ".join(player_final)
    }


# ============================================================================
# Parallel Simulation
# ============================================================================

def run_batch(args: tuple) -> list[dict]:
    """Run a batch of rounds in a single process.

    Args:
        args: Tuple of (threshold, start_round, batch_size, base_seed)

    Returns:
        list[dict]: Results for all rounds in this batch.
    """
    threshold, start_round, batch_size, base_seed = args
    results = []

    for i in range(batch_size):
        round_id = start_round + i
        results.append(play_round(threshold, round_id, base_seed))

    return results


def run_simulation(threshold: int, num_rounds: int = 10000,
                   num_workers: int = None) -> list[dict]:
    """Run parallel simulation for a given strategy.

    Args:
        threshold: Stand threshold (16, 17, or 18).
        num_rounds: Total number of rounds to simulate.
        num_workers: Number of parallel processes. Auto-detect if None.

    Returns:
        list[dict]: All round results.
    """
    if num_workers is None:
        num_workers = mp.cpu_count()

    # Calculate batch size per worker
    batch_size = (num_rounds + num_workers - 1) // num_workers
    base_seed = threshold * 1_000_000

    # Prepare arguments for each batch
    args_list = []
    for i in range(num_workers):
        start_round = i * batch_size + 1
        actual_batch_size = min(batch_size, num_rounds - (i * batch_size))
        if actual_batch_size <= 0:
            break
        args_list.append((threshold, start_round, actual_batch_size, base_seed))

    # Run batches in parallel
    print(f"  Strategy {threshold}: Running {num_rounds} rounds on {len(args_list)} processes...")
    start_time = time.time()

    with mp.Pool(processes=num_workers) as pool:
        batch_results = pool.map(run_batch, args_list)

    # Flatten results
    results = []
    for batch in batch_results:
        results.extend(batch)

    elapsed = time.time() - start_time
    print(f"  Strategy {threshold}: {len(results)} rounds in {elapsed:.2f}s")

    return results


# ============================================================================
# CSV Export
# ============================================================================

def save_to_csv(results: list[dict], filepath: Path) -> None:
    """Save simulation results to a CSV file."""
    if not results:
        print(f"Warning: No results to save for {filepath}")
        return

    headers = [
        "round_id", "strategy", "player_total", "dealer_total",
        "player_bust", "dealer_bust", "player_blackjack",
        "player_hit_count", "dealer_hit_count", "result", "profit",
        "player_initial", "player_final"
    ]

    with open(filepath, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=headers)
        writer.writeheader()
        writer.writerows(results)

    print(f"  Saved {len(results)} rows to {filepath}")


# ============================================================================
# Main Entry Point
# ============================================================================

def main() -> None:
    """Run simulations for all three strategies."""
    print("=" * 60)
    print("BLACKJACK STRATEGY SIMULATOR")
    print("Comparing stand-at-16, stand-at-17, and stand-at-18")
    print("=" * 60)

    # ========================================================================
    # MODIFY STRATEGIES HERE
    # ========================================================================
    # To test different strategies, modify the list below.
    # Example strategies: [15, 16, 17, 18, 19, 20]
    # To test just two: [13, 17]
    # To test a custom range: list(range(14, 21))
    # ========================================================================
    strategies = [16, 17, 18]
    num_rounds = 10000
    output_dir = Path(__file__).parent

    all_results = {}

    for threshold in strategies:
        results = run_simulation(threshold, num_rounds)
        all_results[threshold] = results

        csv_path = output_dir / f"blackjack_strategy_{threshold}.csv"
        save_to_csv(results, csv_path)

    # Print summary statistics
    print("\n" + "=" * 60)
    print("SUMMARY STATISTICS")
    print("=" * 60)

    for threshold in strategies:
        results = all_results[threshold]
        total = len(results)

        wins = sum(1 for r in results if r["result"] == "win")
        loses = sum(1 for r in results if r["result"] == "lose")
        pushes = sum(1 for r in results if r["result"] == "push")
        busts = sum(1 for r in results if r["player_bust"])

        total_profit = sum(r["profit"] for r in results)
        avg_profit = total_profit / total if total else 0

        print(f"\nStand at {threshold}:")
        print(f"  Rounds: {total}")
        print(f"  Win:  {wins:5d} ({wins/total*100:.1f}%)")
        print(f"  Lose: {loses:5d} ({loses/total*100:.1f}%)")
        print(f"  Push: {pushes:5d} ({pushes/total*100:.1f}%)")
        print(f"  Bust: {busts:5d} ({busts/total*100:.1f}%)")
        print(f"  Net profit: {total_profit:+d}")
        print(f"  Avg profit: {avg_profit:+.4f} per round")

    print("\nAll results saved to blackjack_strategy_{16,17,18}.csv")
    print("\nNew columns added: player_initial (starting two cards), player_final (final hand)")


if __name__ == "__main__":
    main()
```

## 7.9.8 Version Comparison

| Feature | Version 1 | Version 2 | Version 3 |
|---------|-----------|-----------|-----------|
| Interactive play | ✅ | ✅ | ❌ |
| CSV logging | ❌ | ✅ | ✅ |
| Parallel computation | ❌ | ❌ | ✅ |
| Configurable strategies | ❌ | ❌ | ✅ |
| Large‑scale simulation | ❌ | ❌ | ✅ |
| Logs player hands | ❌ | ✅ | ✅ |
| Use case | Entertainment | Accumulating real‑play data | Batch‑generating training data |