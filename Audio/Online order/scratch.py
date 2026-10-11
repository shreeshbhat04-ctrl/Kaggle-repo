import pandas as pd
df = pd.read_csv(r"C:\Users\shree\intern\kaggle\Practise\Online food order\dataset\online food delivery dataset.csv")
df["equal"]=df.iloc[:,11]==df.iloc[:,13]
df["diff"]=df.iloc[:,11]!=df.iloc[:,13]
df["check"]=df["Feedback"].map({
    "Positive":"Yes",
    "Negative":"No"
})
df["diff1"]=df.iloc[:,11]!=df["check"]
print(df["equal"].sum())
print(df["diff1"].sum())
print(df.iloc[:,11].head())