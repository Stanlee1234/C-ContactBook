"""Compile and test the public add/list flow using only temporary data files."""
import csv
import pathlib
import subprocess
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[1]


class RoundTripTest(unittest.TestCase):
    def test_add_and_list(self):
        with tempfile.TemporaryDirectory() as temporary:
            temporary = pathlib.Path(temporary)
            binary = temporary / 'contactbook'
            data = temporary / 'contacts.csv'
            subprocess.run(['cc', '-std=c11', '-Wall', '-Wextra', '-Werror',
                            str(ROOT / 'contactbook.c'), '-o', str(binary)], check=True)
            contacts = [
                ['Jane Doe', '123 Main St, Apt 4', 'jane@example.com', '555-0100'],
                ['John Doe', '', 'john@example.com', '555-0200'],
                ['Jane "JJ" Doe', '', '', ''],
                ['"' * 99, '"' * 99, '"' * 99, '"' * 99],
            ]
            user_input = ''.join('a\n' + '\n'.join(contact) + '\n'
                                 for contact in contacts[:3]) + 'q\n'
            subprocess.run([str(binary), str(data)], input=user_input,
                           text=True, capture_output=True, check=True)
            # Max prompt length is tested via a file row, not the UI's separate
            # overlong-input behavior.
            with data.open(newline='') as handle:
                self.assertEqual(list(csv.reader(handle)), contacts[:3])
            with data.open('a', newline='') as handle:
                csv.writer(handle).writerow(contacts[3])
                handle.write('Legacy,,legacy@example.com,555-0300\n')
                handle.write('bad,extra,columns,are,rejected\n')
                handle.write('x' * 1000 + '\n')
                handle.write('After,long@example.com,,555-0400\n')
            output = subprocess.run([str(binary), str(data)], input='l\nq\n',
                                    text=True, capture_output=True, check=True).stdout
            for contact in contacts:
                block = ''.join(f'{label}: {value}\n' for label, value in zip(
                    ['Name', 'Address', 'Email', 'Number'], contact))
                self.assertIn(block, output)
            self.assertIn('Name: Legacy\nAddress: \nEmail: legacy@example.com\nNumber: 555-0300', output)
            self.assertIn('Name: After\nAddress: long@example.com\nEmail: \nNumber: 555-0400', output)
            self.assertNotIn('Name: bad\n', output)


if __name__ == '__main__':
    unittest.main()
