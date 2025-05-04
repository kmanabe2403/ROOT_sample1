{
    int n=50;//ヒストグラムの分割数
    double min=100;//fittingして表示するデータの最小値
    double max=17000;//fittingして表示するデータの最大値(test_generate_data.txtの場合にはtailでほぼ0なので,tailを消去しているが,実際の実験データの場合にはもっと長くしてもよい?(ノイズについてはns単位なので結構長くしても大丈夫そう))
    
    string filename="data9pre.dat";//データのファイル名
    double a0=250.0,b0=2000.0,c0=10.0;//fittingの初期値
    TH1D* hist = new TH1D("hist", "muon decay curve", n, min, max);
    ifstream ifs(filename);
    double a,b,c;

    while( ifs>>a>>b>>c ){//実際のデータではコメントアウトを消す
        if(min<c&&c<max){
            hist->Fill(c);
        }
    }

    TF1 *f = new TF1( "f", "[0]*exp(-x/[1])+[2]",min,max);
    f->SetParameters(a0,b0,c0);
    f->SetParName(0, "N(0)");
    f->SetParName(1, "#tau[ns]");
    f->SetParName(2, "bkg");
    hist->Fit(f);
    gStyle->SetOptFit();
    gStyle->SetOptFit(0111);
    // キャンバスを作成してヒストグラムを描画
    TCanvas* c1 = new TCanvas("c1", "Histogram Canvas", 800, 600);
    hist->Draw();
    hist->Draw("same e1");
    hist->GetXaxis()->SetTitle("Time[ns]");
    hist->GetYaxis()->SetTitle("Counts");
    f->Draw("same"); // フィット結果をヒストグラムと重ねて表示
    // PNGファイルとして保存
    c1->SaveAs("fitting1.png");
}