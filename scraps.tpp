
// Function to print a piece
template<typename T>
void print_table(T const& table)
{
    cout << __func__ << endl;
    for (typename T::size_type i = 0; i < table.size(); i++) {

        for (typename T::size_type j = 0; j < table[i].size(); j++) {

            bool occupied = table[i][j];
            cout << (occupied ? 'o' : '.') << ' ';
        }
        cout << endl;
    }
}
