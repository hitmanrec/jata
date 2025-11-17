<template>
	<button class="category-button">
		<slot></slot>
	</button>
</template>

<script>
export default {
	name: 'category-button',
	props: {
		category: {
			type: Object,
			default: () => ({name: '', id: 0, removed: false})
		}
	}
};
</script>

<style scoped>

</style>