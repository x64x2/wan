package cmd

func reverse(elements []string) []string {
	for i := 0; i < len(elements)/2; i++ {
		j := len(elements) - i - 1
		elements[i], elements[j] = elements[j], elements[i]
	}
	return elements
}
