import importlib.util
from pathlib import Path
import unittest

path=Path(__file__).parents[1]/'tools/telemetry_receiver.py'
spec=importlib.util.spec_from_file_location('receiver',path);receiver=importlib.util.module_from_spec(spec);spec.loader.exec_module(receiver)


class Tests(unittest.TestCase):
    def test_valid_sample(self):
        self.assertEqual(receiver.parse_packet('PKT:P=-30.50,R=36.00,A=1.00'),{'pitch':-30.5,'roll':36.0,'accel':1.0})
    def test_legacy_greeting(self):
        self.assertEqual(receiver.parse_packet('I am Esp8266'),{'type':'greeting'})
    def test_malformed_or_nonfinite_samples(self):
        for sample in ('PKT:P=1,R=2','PKT:P=1,R=2,A=1x','PKT:P=1e999,R=0,A=1','x'*257):
            with self.subTest(sample=sample[:30]),self.assertRaises(ValueError):receiver.parse_packet(sample)


if __name__=='__main__':unittest.main()
