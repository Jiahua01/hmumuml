import ROOT
import array

def create_histogram(file_paths, tree_name, var_to_cut, cut_range, var_to_plot, hist_name, bin_edges, Weights):
    bin_array = array.array('d', bin_edges)
    hist = ROOT.TH1F(hist_name, f"Distribution of {var_to_plot}", len(bin_edges)-1, bin_array)
    hist.Sumw2()

    for file_path in file_paths:
        print(f"Opening file: {file_path}")
        file = ROOT.TFile.Open(file_path)
        if not file or not file.IsOpen():
            print(f"Failed to open file: {file_path}")
            continue

        tree = file.Get(tree_name)
        if not tree:
            print(f"Tree '{tree_name}' not found in {file_path}")
            file.Close()
            continue

        print(f"File {file_path} opened successfully, processing {tree.GetEntries()} entries.")

        # Print example entries to debug
        #tree.Show(0)  # Print the first entry
        #tree.Scan("bdt_score_t:diMufsr_rc_mass", "", "colsize=10 col=bdt_score_t diMufsr_rc_mass")  # Show values

        # Define cut and draw command
        #SR
        #cut_str = f"{Weights}*(({var_to_cut} >= {cut_range[0]} && {var_to_cut} <= {cut_range[1]}))"
        #SB
        cut_str = f"{Weights}*(({var_to_cut} >= {cut_range[0]} && {var_to_cut} <= {cut_range[1]})||(({var_to_cut} >= {cut_range[2]} && {var_to_cut} <= {cut_range[3]})))"#SB
        temp_hist_name = f"temp_{hist_name}"
        temp_hist = ROOT.TH1F(temp_hist_name, f"Temp Histogram for {var_to_plot}", len(bin_edges)-1, bin_array)
        temp_hist.Sumw2()

        draw_cmd = f"{var_to_plot} >> {temp_hist_name}"
        print(f"Draw command: {draw_cmd}, Cut: {cut_str}")

        tree.Draw(draw_cmd, cut_str, "goff")
        print(f"Integral of temporary histogram: {temp_hist.Integral()}")

        # Add temporary histogram contents to main histogram
        hist.Add(temp_hist)

        file.Close()
        print(f"Integral after adding from {file_path}: {hist.Integral()}")

    return hist

def main():
    # bdt
    #bin_edges = [0.0, 0.05, 0.15, 0.24, 0.34, 0.4, 0.51, 0.6, 0.7, 0.8, 1.0] #105To160
    #bin_edges = [0.0, 0.03, 0.08, 0.14, 0.25, 0.35, 0.46, 0.55, 0.68, 0.8,1.0] #105To160 noqgl
    #bin_edges = [0.0, 0.03, 0.1, 0.19, 0.29, 0.39, 0.5, 0.63, 0.7, 0.8, 1.0] #105-160 +ggh
    bin_edges =[0.0,0.06,0.12,0.2,0.3,0.41,0.49,0.58,0.64,0.78,1]
    # dnn
    #bin_edges = [0.0, 0.09, 0.16, 0.2, 0.28, 0.55, 0.64, 0.72, 0.88, 0.96, 1.0]
    #bin_edges = [0.0, 0.52, 0.59, 0.63, 0.71, 0.73, 0.74, 0.77, 0.82, 0.92, 1.0]
    #bin_edges = [0.0, 0.58, 0.67, 0.77, 0.81, 0.83, 0.87, 0.92, 0.95, 1.0]#used
    # BDT with jet1_qgl jet2_qgl
    #path_ = "outputs_0624/two_jet/"
    #path_ = "outputs_0624_SB_HM125/two_jet/"
    # BDT without jet1_qgl jet2_qgl
    path_ = "/eos/user/j/jiahua/vbfhmm/MC_guo/outputs/two_jet/"
    #path_ = "outputs_0730_SB_HM125/two_jet/"

    # DNN with jet1_qgl jet2_qgl
    #path_ = "outputs_dnn_0628/two_jet/"
    #path_ = "outputs_dnn_0628_SB_HM125/two_jet/"
    # DNN without jet1_qgl jet2_qgl
    #path_ = "outputs_dnn_0730/two_jet/"
    #path_ = "outputs_dnn_0730_SB_HM125/two_jet/"
    categories = {
        "vbf": ["vbf.root"],
        "ggh": ["ggh.root"],
        "dy": ["dy.root"],
        #"DY01J": ["DY_105To160_gen01J.root"],
        #"DY2J": ["DY_105To160_genOE2J.root"],
        "ewk": ["ewk.root"],
        #"DY": ["DY_50FxFx.root"],
        #"DY": ["DY_50MLM.root"],
        #"EWKZ": ["EWK_LLJJ_M50.root"],
        "top": ["top.root"],
        "diboson": ["diboson.root"],
        "data_obs": ["data.root"],
    }

    histograms = {}
    for category, files in categories.items():
        full_paths = [path_ + file for file in files]
        # wrong input sig
        #if category == "VBFHmm":
        #    histograms[category] = create_histogram(full_paths, "test", "diMufsr_rc_mass", [115, 135], "bdt_score_t", f"{category}", bin_edges, "eventWeight*6.373435026631877e-06/9.398e-05")
        #else:
        #    histograms[category] = create_histogram(full_paths, "test", "diMufsr_rc_mass", [115, 135], "bdt_score_t", f"{category}", bin_edges, "eventWeight")
        # SR: outputs_0624/two_jet/
        #histograms[category] = create_histogram(full_paths, "test", "H_mass", [115, 135], "bdt_score_t", f"{category}", bin_edges, "weight")
        # SB: outputs_0624_SB_HM125/two_jet/
        histograms[category] = create_histogram(full_paths, "test", "H_mass", [110, 115, 135, 150], "bdt_score_t", f"{category}", bin_edges, "weight")
        print("--------------------------")

    #SR
    #output_file = ROOT.TFile("bdtscore_SR.root", "RECREATE")
    #SB
    output_file = ROOT.TFile("bdtscore_SB.root", "RECREATE")
    for hist in histograms.values():
        hist.Write()
    output_file.Close()

if __name__ == "__main__":
    main()

