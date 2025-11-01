<!-- simplified item representation for item list -->
<template>
	<div class="item">
		<div class="item-content">
			<div class="title">
				{{ item.title }}
			</div>
			<div v-if="item.tags.length > 0" class="tags">
				Tags: {{ item.tags.join(', ') }}
			</div>
			<div class="status">
				Status: {{ item.status }}
			</div>
			<div class="expectation">
				Expectation: {{ item.expectation }}
			</div>
			<div class="rating">
				Rating: {{ item.rating }}
			</div>
		</div>
		<div class="item-btns">
			<small-button class="delete-button"
				@click="$emit('delete-item', item)"
			>
				Delete
			</small-button>
		</div>
	</div>
</template>

<script>
export default {
	props: {
		item: {
			type: Object,
			required: true
		}
	}
}
</script>

<style scoped>
.item{
	padding: 10px;
	border: 2px solid var(--accent-col);
	border-radius: var(--big-border-rad);
	display: flex;
	flex-direction: column;
}

.item-content{
	display: flex;
	flex-direction: column;
	justify-content: space-between;
}

.title{
	font-family: var(--strong-font);
	font-weight: var(--main-font-weight);
	font-size: 1.2em;
	margin-bottom: 5px;
}

.status{
	align-self: flex-end;
}

.rating{
	align-self: flex-end;
}

.expectation{
	align-self: flex-end;
}

.item-btns{
	margin-top: 10px;
	display: flex;
	flex-direction: column;
	align-items: flex-end;
}
</style>