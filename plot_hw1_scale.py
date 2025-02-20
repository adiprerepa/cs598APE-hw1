import pandas as pd
import matplotlib.pyplot as plt

# Define the dataset, ignoring 16-core data
data = {
    "Threads": [1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8],
    "Experiment": ["elephant", "globe", "pianoroom"] * 8,
    "Elapsed(s)": [21.515369315, 0.843254842, 0.340007747,
                   10.913087651, 0.635209968, 0.177003873,
                   7.153431303, 0.594581246, 0.117647640,
                   5.445826830, 0.558810095, 0.091089516,
                   4.820915311, 0.632970865, 0.074876040,
                   3.881814886, 0.533278265, 0.064717345,
                   3.569863252, 0.575452777, 0.057939010,
                   3.249703329, 0.574516911, 0.055345464]
}

# Convert to DataFrame
df = pd.DataFrame(data)

# Compute speedup: Speedup = T(1 thread) / T(N threads)
df["Speedup"] = df.apply(
    lambda row: df[(df["Experiment"] == row["Experiment"]) & (df["Threads"] == 1)]["Elapsed(s)"].values[0] / row["Elapsed(s)"], 
    axis=1
)

# Generate separate PDF plots for each experiment
for experiment in df["Experiment"].unique():
    subset = df[df["Experiment"] == experiment]
    
    plt.figure(figsize=(8, 6))
    
    # Compute ideal speedup line
    ideal_threads = sorted(subset["Threads"].unique())
    ideal_speedup = [t for t in ideal_threads]

    # Plot actual speedup
    plt.plot(subset["Threads"], subset["Speedup"], 'o--', label="Measured Speedup", color="tab:blue")

    # Plot ideal linear speedup
    plt.plot(ideal_threads, ideal_speedup, 'r-', label="Ideal Linear Speedup", linewidth=2)

    plt.title(f"Speedup: {experiment.capitalize()}")
    plt.xlabel("Number of Threads")
    plt.ylabel("Speedup Factor")
    plt.grid(True, linestyle="--")
    plt.legend()

    # Save as PDF
    pdf_filename = f"speedup_{experiment}.pdf"
    plt.savefig(pdf_filename, format="pdf")
    plt.close()

print("Speedup plots saved as PDFs: speedup_elephant.pdf, speedup_globe.pdf, speedup_pianoroom.pdf")
