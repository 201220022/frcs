# pyinstaller --onefile --name=xlsReader --hidden-import="pandas,openpyxl" main.py
import pandas as pd
import sys

def process_excel_to_csv(excel_file_name):
    xls = pd.ExcelFile("道友/" + excel_file_name + ".xlsx")
    for sheet_name in xls.sheet_names:
        df = pd.read_excel(xls, sheet_name=sheet_name)
        csv_file_name = f'py/{excel_file_name}-{sheet_name}.csv'
        df.to_csv(csv_file_name, index=False, encoding='gbk')

    xls.close()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        #process_excel_to_csv("欣酱")
        sys.exit(0)

    excel_file_name = sys.argv[1]
    process_excel_to_csv(excel_file_name)
